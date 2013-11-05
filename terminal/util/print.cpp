/*
 * print.c
 *
 *  Created on: 2012-11-7
 *      Author: zhangbo
 */

#include "print.h"
#include <windows.h>
#include <WinIo.h>
#include "../terminal/windows/message.h"
#include "../terminal/putty.h"
typedef struct _LPT {
	unsigned long data;
	unsigned long stat;
	unsigned long ctrl;
} LPT;

LPT lpt;
void* lptopen(unsigned long port) {
	char drv[] = "./WinIo.sys";
	if (InstallWinIoDriver(drv)) {
		if (InitializeWinIo()) {
			LPT* pLpt = &lpt;
			pLpt->data = port;
			pLpt->stat = port + 1;
			pLpt->ctrl = port + 2;
			return pLpt;
		}
	}
	return 0;
}

static bool wait(LPT* plpt) {
	byte status = 0;
	DWORD dwPortVal = 0;
	bool done = false;
	int count = 0;
	while ((!done) && (count < 10)) {
		count++;
		GetPortVal(plpt->stat, &dwPortVal, 1);
		dwPortVal = dwPortVal & 0x0FF;
		status = 0;
		status = (byte) dwPortVal;

		if ((status & 0x80) == 0x00) {
			//判断S7位为低电平说明打印机不正常或忙状态,异常为0
			done = false;
		} else if ((status & 0x20) == 0x20) {
			//判断S5位为低电平说明缺纸状态,异常为1
			done = false;
		} else if ((status & 0x10) == 0x00) {
			//当打印机处于无效状态时，该信号就会变成低电平
			done = false;
		} else if ((status & 0x08) == 0x00) {
			//判断S3位为低电平说明打印机未知错误,异常为0
			done = false;
		} else {
			done = true;
		}
	}
	return done;
}

unsigned long lptwrite(void* lpt, const void* data, int len) {
	LPT* plpt = (LPT*) lpt;
	DWORD cmdBy[8];
	DWORD readCMD = 0;

	while (wait(plpt) == false) {
		if (MessageBox(hwnd, ERROR_PRINT, TITLE_POINT,
				MB_ICONERROR | MB_OKCANCEL) == IDCANCEL)
			return 1;
		Sleep(10);
	}

	for (int i = 0; i < len; i++) {
		//打开选通1脚
		GetPortVal(plpt->ctrl, &readCMD, 1);
		cmdBy[1] = readCMD | 0x09;
		SetPortVal(plpt->ctrl, cmdBy[1], 1);
		SetPortVal(plpt->data, *(((char*) data) + i), 1);
		GetPortVal(plpt->ctrl, &readCMD, 1);
		cmdBy[2] = readCMD & 0xFE;
		SetPortVal(plpt->ctrl, cmdBy[2], 1);

		while (wait(plpt) == false) {
			Sleep(10);
		}
	}

	return 0;

}

void lptclose(void* lpt) {
	ShutdownWinIo();
}
