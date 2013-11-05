/*
 * ClientThread.cpp
 *
 *  Created on: 2013-1-23
 *      Author: zhangbo
 */

#include "ClientThread.h"

#include "./util/print.h"
#include "./util/comapi.h"
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
using namespace std;

client ClientThread::coms[MAX_COM_COUNT];
void* ClientThread::printer;

ClientThread::ClientThread(SOCKET socket) {
	this->socket = socket;
}

ClientThread::~ClientThread() {
}

void ClientThread::SetSocket(SOCKET socket) {
	this->socket = socket;
}

void ClientThread::Initialize() {
	for (int i = 0; i < MAX_COM_COUNT; i++) {
		InitializeCriticalSection(&(coms[i].critsec));
	}
	printer = lptopen(0x378);
	if (printer == (void*) 0) {
		MessageBox(NULL, "printer open error", "", MB_ICONERROR | MB_OK);
		return;
	}
}

void ClientThread::Uninitialize() {
	for (int i = 0; i < MAX_COM_COUNT; i++) {
		DeleteCriticalSection(&(coms[i].critsec));
	}
	lptclose(printer);
}

int ClientThread::Read(unsigned int socket, char* buffer, int len) {
	int rvCount = 0;
	do {
		rvCount = recv(socket, buffer, len, 0);
		if (rvCount <= 0) {
			return 0;
		}
	} while (false);
	return rvCount;
}

void ClientThread::Run() {
	char buffer[1024];
//	while (1) {
//		int rvCount = recv(socket, buffer, 1024, 0);
//		if (rvCount > 0) {
//			//lptwrite(printer, buffer, rvCount);
//		} else if (rvCount == SOCKET_ERROR) {
//			return;
//		} else {
//			printf("%d", rvCount);
//			return;
	while (1) {
		int rvCount = recv(socket, buffer, 1024, 0);
		if (rvCount <= 0) {
			return;
		}
		lptwrite(printer, buffer, rvCount);
	}
//
//	char buffer[1024];
//	Read(socket, buffer, 5);
//	unsigned char screen = (buffer[3] - '0') * 10 + (buffer[4] - '0');
//	buffer[3] = 0;
//	if (strcmp(buffer, "PRT") == 0) {
//		while (1) {
//			int rvCount = recv(socket, buffer, 1024, 0);
//			if (rvCount <= 0) {
//				return;
//			}
//			lptwrite(printer, buffer, rvCount);
//		}
//	} else if (strcmp(buffer, "COM") == 0) { //COM
//		HANDLE com = NULL;
//		COMPARAM param;
//		unsigned char buf[1024];
//		int len = 0;
//		while (1) {
//			if (Read(socket, buffer, 3) == 0) {
//				if (com != NULL) {
//					coms[param.bComId].screen = 0;
//					coms[param.bComId].lock = 0;
//					LeaveCriticalSection(&(coms[param.bComId].critsec));
//				}
//				break;
//			}
//			buffer[3] = 0;
//			if (strcmp(buffer, "SET") == 0) {
//				int rvCount = this->Read(socket, (char*) &param, sizeof(param));
//				if (rvCount <= 0) {
//					return;
//				}
//				EnterCriticalSection(&(coms[param.bComId].critsec));
//				coms[param.bComId].screen = screen;
//				coms[param.bComId].lock = 1;
//			} else if (strcmp(buffer, "OPE") == 0) {
//				com = com_open(&param);
//			} else if (strcmp(buffer, "CLO") == 0) {
//				com_close(com);
//				com = NULL;
//				coms[param.bComId].screen = 0;
//				coms[param.bComId].lock = 0;
//				LeaveCriticalSection(&(coms[param.bComId].critsec));
//			} else if (strcmp(buffer, "REA") == 0) {
//				unsigned long size = com_read(com, buf, 1024);
//				send(socket, (const char*) &size, sizeof(size), 0);
//				if (size > 0) {
//					send(socket, (const char*) buf, size, 0);
//				}
//			} else if (strcmp(buffer, "WRI") == 0) {
//				Read(socket, (char*) &len, sizeof(int));
//				Read(socket, (char*) buf, len);
//				com_write(com, buf, len);
//			}
//		}
//	} else if (strcmp(buffer, "LST") == 0) {
//		//列出所有占用的COM口
//	} else if (strcmp(buffer, "RST") == 0) {
//		//重启指定的COM口
//	}
	return;

//	Read(socket, buffer, 5);
//	unsigned char screen = (buffer[3] - '0') * 10 + (buffer[4] - '0');
//	buffer[3] = 0;
//	if (strcmp(buffer, "PRT") == 0) {
//		while (1) {
//			int rvCount = recv(socket, buffer, 1024, 0);
//			if (rvCount <= 0) {
//				return;
//			}
//			lptwrite(printer, buffer, rvCount);
//		}
//	} else if (strcmp(buffer, "COM") == 0) { //COM
//		HANDLE com = NULL;
//		COMPARAM param;
//		unsigned char buf[1024];
//		int len = 0;
//		while (1) {
//			if (Read(socket, buffer, 3) == 0) {
//				if (com != NULL) {
//					coms[param.bComId].screen = 0;
//					coms[param.bComId].lock = 0;
//					LeaveCriticalSection(&(coms[param.bComId].critsec));
//				}
//				break;
//			}
//			buffer[3] = 0;
//			if (strcmp(buffer, "SET") == 0) {
//				int rvCount = this->Read(socket, (char*) &param, sizeof(param));
//				if (rvCount <= 0) {
//					return;
//				}
//				EnterCriticalSection(&(coms[param.bComId].critsec));
//				coms[param.bComId].screen = screen;
//				coms[param.bComId].lock = 1;
//			} else if (strcmp(buffer, "OPE") == 0) {
//				com = com_open(&param);
//			} else if (strcmp(buffer, "CLO") == 0) {
//				com_close(com);
//				com = NULL;
//				coms[param.bComId].screen = 0;
//				coms[param.bComId].lock = 0;
//				LeaveCriticalSection(&(coms[param.bComId].critsec));
//			} else if (strcmp(buffer, "REA") == 0) {
//				unsigned long size = com_read(com, buf, 1024);
//				send(socket, (const char*) &size, sizeof(size), 0);
//				if (size > 0) {
//					send(socket, (const char*) buf, size, 0);
//				}
//			} else if (strcmp(buffer, "WRI") == 0) {
//				Read(socket, (char*) &len, sizeof(int));
//				Read(socket, (char*) buf, len);
//				com_write(com, buf, len);
//			}
//		}
//	} else if (strcmp(buffer, "LST") == 0) {
//		列出所有占用的COM口
//	} else if (strcmp(buffer, "RST") == 0) {
	//重启指定的COM口
//	}
//	return;
}
