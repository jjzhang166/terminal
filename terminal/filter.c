/*
 * Extends.cpp
 *
 *  Created on: 2012-9-26
 *      Author: zhangbo
 */

#include "filter.h"
#include "windows/message.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "putty.h"
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#include "util/lock.h"
#include "util/com.h"
#include "util/log.h"

static int setup = 0;
int com_enable = 0;
static int output = 0;
static int direct_mode = 1;
static int line_mode = 0;
static int running = 0;
extern void *backhandle;
extern Backend *back;

int com_setup(char com, int baudRate, int parity, int byteSize, int stopBits) {
	COMPARAM p;
	p.bEvtChar = 0x0d;
	p.fBinary = 1;

	switch (baudRate) {
	case 0:
		p.dwBaudRate = 9600;
		break;
	case 1:
		p.dwBaudRate = 4800;
		break;
	case 2:
		p.dwBaudRate = 2400;
		break;
	case 3:
		p.dwBaudRate = 1200;
		break;
	case 4:
		p.dwBaudRate = 900;
		break;
	case 5:
		p.dwBaudRate = 600;
		break;
	case 6:
		p.dwBaudRate = 300;
		break;
	case 7:
		p.dwBaudRate = 150;
		break;
	case 8:
		p.dwBaudRate = 75;
		break;
	case 9:
		p.dwBaudRate = 800;
		break;
	default:
		p.dwBaudRate = 9600;
		break;
	}

	switch (parity) {
	case 0: //none
		p.bParity = 0;
		break;
	case 1: //1
		p.bParity = 1;
		break;
	case 2:
		p.bParity = 2;
		break;
	default:
		p.bParity = 0;
		break;
	}

	switch (byteSize) {
	case 0:
		p.bByteSize = 8;
		break;
	case 1:
		p.bByteSize = 7;
		break;
	default:
		p.bByteSize = 8;
		break;
	}

	switch (stopBits) {
	case 0:
		p.bStopBits = 0; //1
		break;
	case 1:
		p.bStopBits = 2; //2
		break;
	default:
		p.bStopBits = 1; //1.5
		break;
	}

	switch (com) {
	case 'Y':
		p.bComId = cfg.com01y;
		if (cfg.useselfsety == 1) {
			p.dwBaudRate = cfg.bandy;
		}
		break;
	case 'Z':
		p.bComId = cfg.com02z;
		if (cfg.useselfsetz == 1) {
			p.dwBaudRate = cfg.bandz;
		}
		break;
	case 'X':
		p.bComId = cfg.com03x;
		if (cfg.useselfsetx == 1) {
			p.dwBaudRate = cfg.bandx;
		}
		break;
	case 'U':
		p.bComId = cfg.com04u;
		if (cfg.useselfsetu == 1) {
			p.dwBaudRate = cfg.bandu;
		}
		break;
	case 'W':
		p.bComId = cfg.com05w;
		if (cfg.useselfsetw == 1) {
			p.dwBaudRate = cfg.bandw;
		}
		break;
	case 'V':
		p.bComId = cfg.com06v;
		if (cfg.useselfsetv == 1) {
			p.dwBaudRate = cfg.bandv;
		}
		break;
	default:
		p.bComId = cfg.com01y;
		if (cfg.useselfsety == 1) {
			p.dwBaudRate = cfg.bandy;
		}
		break;
	}

	current_com_setup(&p);
	setup = 1;
	if (com_enable && setup) {
		current_com_open();
	}
	return 0;
}

#define COM_READ_BUFFER_SIZE 1024

static void com_send(unsigned char* data, unsigned long len) {
	if (!direct_mode) {
		int i = 0, j = 0;
		char* pout = malloc(len * 3);
		for (i = 0; i < len; i++) {
			pout[j++] = 0x80;
			pout[j++] = data[i];
			pout[j++] = 0x81;
		}
		back->send(backhandle, pout, len * 3);
		free(pout);
	} else {
		back->send(backhandle, (char*) data, len);
	}
}

static unsigned char inBuffer[COM_READ_BUFFER_SIZE];
static unsigned long inLen = 0;
static unsigned char tempBuffer[COM_READ_BUFFER_SIZE * 10];
static long tempLen = 0;
static unsigned long WINAPI com_read_thread(void* device) {
	log_write_str(COM_INFO, "com_read_thread start");
	while (running) {
		Sleep(5);
		if (!com_enable) {
			continue;
		}
		inLen = current_com_read(inBuffer, COM_READ_BUFFER_SIZE);
		if (inLen == 0) {
			continue;
		}
		if (line_mode) {
			int last = 0, i = 0;

			for (i = 0; i < inLen; i++) {
				if (inBuffer[i] == 0x0D) {
					memcpy(tempBuffer + tempLen, inBuffer + last, i + 1 - last);
					tempLen += i + 1 - last;
					last = i + 1;
					com_send(tempBuffer, tempLen);
					tempLen = 0;
				}
			}
			memcpy(tempBuffer + tempLen, inBuffer + last, i - last);
			tempLen += i - last;
		} else {
			com_send(inBuffer, inLen);
		}
	}
	log_write_str(COM_INFO, "com_read_thread ended");

	return 0;
}

void com_read_proc_start() {
	if (running == 0) {
		CreateThread(NULL, 0, com_read_thread, NULL, 0, 0);
		running = 1;
	}
}

void com_read_proc_stop() {
	running = 0;
}

void com_set_enable(int state) {
	com_enable = state;
	if (com_enable && setup) {
		if (current_com_open() == -1) {
			MessageBoxA(NULL, OPENCOMFAIL, APPLICATION_NAME, MB_OK);
		}
	} else {
		current_com_close();
		comchannel = 0;
		setup = 0;
	}
}

void com_set_output(int state) {
	output = state;
	if (state) {
		log_write_str(COM_INFO, "com out 1");
	} else {
		log_write_str(COM_INFO, "com out 0");
	}
}

int com_get_output() {
	return output && com_enable;
}

void com_set_line_mode(int state) {
	line_mode = state;
	if (state) {
		log_write_str(COM_INFO, "line mode 1");
	} else {
		log_write_str(COM_INFO, "line mode 0");
	}
}

void com_set_direct_mode(int state) {
	direct_mode = state;
	if (state) {
		log_write_str(COM_INFO, "direct mode 1");
	} else {
		log_write_str(COM_INFO, "direct mode 0");
	}
}

void com_receive(unsigned char* data, unsigned long len) {
	current_com_write(data, len);
}
