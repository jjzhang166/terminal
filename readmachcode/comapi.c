/*
 * comapi.cpp
 *
 *  Created on: 2012-9-20
 *      Author: zhangbo
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "comapi.h"
#define SENDLEN 120
unsigned char senddata[SENDLEN];
int sendlen;

//unsigned char recbuf[1024];

unsigned char currentrec[1024];
int currentlen = 0;
int standlen = 0;
//#include "../filter.h"
void* com_open(LPCOMPARAM param) {
	char com[16];
	DCB dcb;
	HANDLE hComm;
	COMMTIMEOUTS timeouts;

	memset(com,0x00,16);
	sprintf(com, "\\\\.\\COM%d", param->bComId);
	//| FILE_FLAG_OVERLAPPED
	hComm = CreateFile(com, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
	if (hComm == INVALID_HANDLE_VALUE) {
		CloseHandle(hComm);
		return NULL;
	}

	//memset(&dcb, 0, sizeof(dcb));

	if (!GetCommState(hComm, &dcb)) {
		CloseHandle(hComm);
		return NULL;
	}

	dcb.BaudRate = param->dwBaudRate;
	dcb.Parity = param->bParity;
	if (!dcb.Parity) {
		dcb.fParity = 0;
	}
	dcb.StopBits = param->bStopBits;
	dcb.ByteSize = param->bByteSize;
	dcb.fBinary = param->fBinary;
	dcb.EvtChar = param->bEvtChar;

	if (!SetCommState(hComm, &dcb)) {
		DWORD error = GetLastError();
		CloseHandle(hComm);
		return NULL;
	}

	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(hComm, &timeouts)) {
		CloseHandle(hComm);
		return NULL;
	}

	SetCommMask(hComm, EV_RXCHAR);
	PurgeComm(hComm,
			PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	return hComm;
}

unsigned long com_read(void* com, unsigned char* buffer, unsigned long bufLen) {
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD dwReadLen = 0;
	OVERLAPPED ov;
	COMSTAT comstat;
	DWORD dwToReadLen = 0;
	memset(&ov, 0, sizeof(OVERLAPPED));

	bResult = ClearCommError(com, &dwError, &comstat);
	if (comstat.cbInQue == 0) {
		return 0;
	}

	dwToReadLen = bufLen;
	if (comstat.cbInQue < bufLen) {
		dwToReadLen = comstat.cbInQue;
	}

	bResult = ReadFile(com, buffer, dwToReadLen, &dwReadLen, NULL);
	if (!bResult) {
		dwError = GetLastError();
		if (dwError == ERROR_IO_PENDING) {
			bResult = GetOverlappedResult(com, &ov, &dwReadLen, TRUE);
			if (!bResult) {
				PurgeComm(com,
						PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR
								| PURGE_RXCLEAR);
				return 0;
			}
		}
	}

//	PurgeComm(com,
//			PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	return dwReadLen;
}

unsigned long com_write(void* com, const unsigned char* data, unsigned long len) {
	BOOL bResult = TRUE;
	DWORD dwWriteLen = 0;
	OVERLAPPED ov;
	memset(&ov, 0, sizeof(OVERLAPPED));

	bResult = WriteFile(com, data, len, &dwWriteLen, NULL);
	if (!bResult) {
		if (GetLastError() == ERROR_IO_PENDING) {
			bResult = GetOverlappedResult(com, &ov, &dwWriteLen, TRUE);
			if (!bResult) {
				return -1;
			}
		}
	}

	return dwWriteLen;
}

int com_close(void* com) {
	CloseHandle(com);
	return 0;
}

int recive(void*com, unsigned char*recbuf) {
	int reclen = 0;
	int i = 0;
	while (reclen < standlen) {
		currentlen = com_read(com, currentrec, 1024);
		if (currentlen == 0) {
			Sleep(20);
			i++;
			if (i == 10) {
				return 0;
			}
		}

		if (currentlen > 0) {
			memcpy(recbuf + reclen, currentrec, currentlen);
			reclen += currentlen;
			memset(currentrec, 0x00, 1024);
			currentlen = 0;
		}

	}

//	printf("rec=%s,len=%d", recbuf, reclen);
	return 1;
}

unsigned long sendprocess(void* com, int control, int len, unsigned char *str) {
	unsigned int sendlen = 0;
	memset(senddata, 0x00, SENDLEN);
	senddata[0] = 0x1b;
	senddata[1] = control;
	switch (control) {
	case 1:
		standlen = 24;
		sendlen = 2;
		break;
	case 0x02:
	case 0x22:
	case 0x42:
	case 0x52:
		senddata[2] = len;
		memcpy(senddata + 3, str, len);
		standlen = 3;
		sendlen = 3 + len;
		break;
	case 3:
		senddata[2] = len;
		memcpy(senddata + 3, str, len);
		sendlen = 3 + len;
		break;
	case 4:
		standlen = 2;
		sendlen = 2;
		break;
	case 5:
		sendlen = 2;
		standlen = 2;
		break;
	case 6:
		senddata[2] = len;
		memcpy(senddata + 3, str, len);
		standlen = 6;
		sendlen = 3 + len;
		break;
	case 7:
		sendlen = 2;
		standlen = 1;
		break;
	default:
		break;
	}
	return com_write(com, senddata, sendlen);
}

void mainmain() {
	/*	unsigned char cs1[] = { 0xAA, 0xE5, 0xAA, 0xE5, 0xAA, 0x02, 0x43, 0x30,
	 0x30, 0x30, 0x30, 0x03 };

	 unsigned char machcodes[40] = "0023c77aeb7351b24ed0d07dc01df4ab";

	 COMPARAM parm;
	 parm.bByteSize = 8;
	 parm.bComId = 4;
	 parm.bParity = 0;
	 parm.bStopBits = 0;
	 parm.dwBaudRate = 4800;
	 parm.fBinary = 1;
	 void* hComm = com_open(&parm);
	 if (hComm == NULL) {
	 MessageBox(NULL, "not exist", "ok",
	 MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1);
	 return;
	 }

	 if (sendprocess(hComm, 2, strlen(machcodes), machcodes) == 0) {
	 MessageBox(NULL, "write error", "ok",
	 MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1);
	 }
	 //	while (1) {
	 recprocess(2, hComm);
	 //	}
	 printf("recive2=%s,len=%d,%d\r\n", recbuf, strlen(recbuf), reclen);
	 fflush(stdout);
	 com_close(hComm);
	 */
//	unsigned char cs1[] = { 0xAA, 0xE5, 0xAA, 0xE5, 0xAA, 0x02, 0x43, 0x30,
//			0x30, 0x30, 0x30, 0x03 };
//	unsigned char cs2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//			0x00, 0x00, 0x00, 0x00 };
//	com_read_proc_start();
//	com_setup('Y',0,0,0,0);
//	com_set_line_mode(0);
//	com_set_direct_mode(1);
//	com_set_enable(1);
//	com_set_output(1);
//	com_receive(cs1,12);//com_write(hComm, cs1, 12);
//	Sleep(1000);
//	com_read_proc_stop();
}
