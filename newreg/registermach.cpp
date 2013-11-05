/*
 * registermach.cpp
 *
 *  Created on: 2012-12-22
 *      Author: xia
 */

#include "registermach.h"
#include <stdio.h>

#include "resources/Resource.h"
#include "../winutil/encrypt.h"
#include "../util/Util.h"
#include "../winutil/getPcMsgApi.h"
#include "iphlpapi.h"
#include "comapi.h"
#include "message.h"
using namespace std;

extern "C" {
extern void mainmain();
//extern char recbuf[1024];
//extern int reclen;
}
unsigned char recbuf[512];
//int reclen = 0;
char machcodes[20];
char registercode[20];
int first = 0;
void* hComm;
registermach::registermach() {
	// TODO Auto-generated constructor stub

}

registermach::~registermach() {
	// TODO Auto-generated destructor stub
}

void registermach::GetMachCode(HWND hwnd, int ID, char *machcodes) {
	char temp[4];
	memset(temp, 0x00, 4);
	int temp1 = GetDlgItemInt(hwnd, ID, 0, FALSE);
	sprintf(temp, "%04d", temp1 % 10000);
	strcat(machcodes, temp);
}

int registermach::GetHash(unsigned char *buf) {
	int hash;
	hash = buf[2];
	hash <<= 8;
	hash += buf[1];
	hash <<= 8;
	hash += buf[0];

	return hash;
}

string registermach::GenerateMach(char * localRegistCod, int type) {
	char Serialnum[256];
	memset(Serialnum, 0x00, 256);
	int rv = 0;
	int control = 0;
	switch (type) {
	case 1:
		rv = getCpuSeleroNumber(Serialnum);
		control = 0x02;
		break;
	case 2:
		rv = getHddSeleroNumber(Serialnum);
		control = 0x22;
		break;
	case 3:
		rv = Util::getMacAddress(Serialnum);
		control = 0x42;
		break;
	}
	sendprocess(hComm, control, strlen(Serialnum), (unsigned char*) Serialnum);
	memset(recbuf, 0x00, 512);
	while (recive(hComm, recbuf) < 2) {
		sendprocess(hComm, control, strlen(Serialnum),
				(unsigned char*) Serialnum);
		memset(recbuf, 0x00, 512);
	}
	int hash = registermach::GetHash(recbuf);
	char temp[4];
	memset(temp, 0x00, 4);
	sprintf(temp, "%04d", hash % 10000);
	strcat(localRegistCod, temp);
	return temp;
}

int InitComCombox(HWND hwnd) {
	unsigned int iCircle;
	HKEY hKey;
	DWORD MaxValueLength;
	DWORD dwValueNumber;
	DWORD cchValueName, dwValueSize = 6;
	char pValueName[32], pCOMNumber[32];
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0,
			KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
		return 0;
	}
	if (RegQueryInfoKeyA(hKey, NULL, NULL, NULL, NULL, NULL, NULL,
			&dwValueNumber, &MaxValueLength, NULL, NULL, NULL) != ERROR_SUCCESS) {
		return 0;
	}
	for (iCircle = 0; iCircle < dwValueNumber; iCircle++) {
		cchValueName = MaxValueLength + 1;
		dwValueSize = 6;
		long lReg = RegEnumValueA(hKey, iCircle, pValueName, &cchValueName,
				NULL, NULL, NULL, NULL);
		if ((lReg != ERROR_SUCCESS) && (lReg != ERROR_NO_MORE_ITEMS)) {
			continue;
		}
		RegQueryValueExA(hKey, pValueName, NULL, NULL, (LPBYTE) pCOMNumber,
				&dwValueSize);
		SendDlgItemMessage(hwnd, IDC_SETCOM, CB_ADDSTRING, 0,
				(LPARAM) pCOMNumber);
	}
	return 1;
}

void AllEnable(HWND hwnd, bool enable) {
	EnableWindow(GetDlgItem(hwnd, IDC_CPUSTR), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_DISKSTR), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_MACSTR), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_REG), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_CURRENT), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_TOTAL), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_LOCAL), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_CLEAR), enable);
	EnableWindow(GetDlgItem(hwnd, IDC_REGIESTCODE), enable);

}

int GetComSet(HWND hwnd, int ID, int add) {
	char ComValue[8];
	memset(ComValue, 0x00, 8);
	GetDlgItemText(hwnd, ID, ComValue, 8);
	return atoi(ComValue + add);
}

static int CALLBACK TestProc(HWND hwnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {
	static unsigned int totaltemp = 0;
	unsigned int currenttemp = 0;
	switch (msg) {
	case WM_INITDIALOG:
		InitComCombox(hwnd);
		AllEnable(hwnd, FALSE);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CONNECT:
			COMPARAM parm;
			parm.bComId = GetComSet(hwnd, IDC_SETCOM, 3);
			hComm = registermach::OpenCom(&parm);
			if (hComm == NULL) {
				MessageBox(NULL, OPENERROR, COMPANY_NAME,
						MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1);
				return -1;
			}

			sendprocess(hComm, 0X04, 0, 0);
			memset(recbuf, 0x00, 512);
			if (recive(hComm, recbuf) == 0) {
				MessageBox(NULL, CONNECTFAIL, COMPANY_NAME,
						MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1);
				return -1;
			}

			totaltemp = recbuf[1] * 256 + recbuf[0];
			SetDlgItemInt(hwnd, IDC_TOTAL, totaltemp, false);
			Sleep(50);
			sendprocess(hComm, 0X05, 0, 0);
			memset(recbuf, 0x00, 512);
			recive(hComm, recbuf);
			currenttemp = recbuf[1] * 256 + recbuf[0];
			SetDlgItemInt(hwnd, IDC_CURRENT, currenttemp, false);
			if (totaltemp <= currenttemp) {
				MessageBox(NULL, UNAVAILABLE, COMPANY_NAME,
						MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1);
				return -1;
			} else {
				AllEnable(hwnd, TRUE);
			}
			SetFocus(GetDlgItem(hwnd, IDC_CPUSTR));
			SendMessageA(GetDlgItem(hwnd, IDC_CPUSTR), EM_LIMITTEXT, 4, 0);
			SendMessageA(GetDlgItem(hwnd, IDC_DISKSTR), EM_LIMITTEXT, 4, 0);
			SendMessageA(GetDlgItem(hwnd, IDC_MACSTR), EM_LIMITTEXT, 4, 0);
			break;

		case IDC_LOCAL: {
			EnableWindow(GetDlgItem(hwnd, IDC_LOCAL), false);
			char localRegistCod[20];
			string temp;
			Sleep(50);
			SetDlgItemTextA(hwnd, IDC_REGIESTCODE, "");
			memset(localRegistCod, 0x00, 20);
			temp = "";
			temp = registermach::GenerateMach(localRegistCod, 1);
			SetDlgItemTextA(hwnd, IDC_CPUSTR, temp.c_str());
			Sleep(50);
			temp = "";
			temp = registermach::GenerateMach(localRegistCod, 2);
			SetDlgItemTextA(hwnd, IDC_DISKSTR, temp.c_str());
			Sleep(50);
			temp = "";
			temp = registermach::GenerateMach(localRegistCod, 3);
			SetDlgItemTextA(hwnd, IDC_MACSTR, temp.c_str());
			SendMessage(hwnd, WM_COMMAND, IDC_REG, 0);


			break;
		}
		case IDC_CLEAR:
			SetDlgItemTextA(hwnd, IDC_CPUSTR, "");
			SetDlgItemTextA(hwnd, IDC_DISKSTR, "");
			SetDlgItemTextA(hwnd, IDC_MACSTR, "");
			SetDlgItemTextA(hwnd, IDC_REGIESTCODE, "");
			EnableWindow(GetDlgItem(hwnd, IDC_REG), true);
			EnableWindow(GetDlgItem(hwnd, IDC_LOCAL), true);
			break;
		case IDOK: {
			if (SendMessageA(GetDlgItem(hwnd, IDC_CPUSTR), WM_GETTEXTLENGTH, 0,
					0) >= 4)
				SetFocus(GetDlgItem(hwnd, IDC_DISKSTR));
			if (SendMessageA(GetDlgItem(hwnd, IDC_DISKSTR), WM_GETTEXTLENGTH, 0,
					0) >= 4)
				SetFocus(GetDlgItem(hwnd, IDC_MACSTR));
			if (SendMessageA(GetDlgItem(hwnd, IDC_MACSTR), WM_GETTEXTLENGTH, 0,
					0) >= 4)
				SetFocus(GetDlgItem(hwnd, IDC_REG));
			break;
		}
		case IDCANCEL:
			EndDialog(hwnd, TRUE);
			break;
		case IDC_REG:
			EnableWindow(GetDlgItem(hwnd, IDC_REG), false);
			registermach::GetMachCode(hwnd, IDC_CPUSTR, machcodes);
			machcodes[4] = '-';
			registermach::GetMachCode(hwnd, IDC_DISKSTR, machcodes);
			machcodes[9] = '-';
			registermach::GetMachCode(hwnd, IDC_MACSTR, machcodes);
			machcodes[14] = '0';
			machcodes[15] = '0';

			sendprocess(hComm, 1, 0, 0);
			memset(recbuf, 0x00, 512);
			while (recive(hComm, recbuf) < 24) {
				sendprocess(hComm, 1, 0, 0);
				memset(recbuf, 0x00, 512);
			}
			char secretkey[3][8];
			memcpy(secretkey[0], recbuf, 8);
			memcpy(secretkey[1], recbuf + 8, 8);
			memcpy(secretkey[2], recbuf + 16, 8);
			char registerkey[1024];
			memset(registerkey, 0x00, 1024);
			encrypt_3des(secretkey[0], secretkey[1], secretkey[2],
					(unsigned char *) machcodes, (unsigned char *) registerkey);
			sendprocess(hComm, 0x52, strlen(registerkey),
					(unsigned char*) registerkey);
			memset(recbuf, 0x00, 512);
			recive(hComm, recbuf);
			sprintf(registercode, "%06d", registermach::GetHash(recbuf));
			SetDlgItemTextA(hwnd, IDC_REGIESTCODE, registercode);
//			Sleep(1000);
			sendprocess(hComm, 0X05, 0, 0);
			memset(recbuf, 0x00, 512);
			recive(hComm, recbuf);
			currenttemp = recbuf[1] * 256 + recbuf[0];
			SetDlgItemInt(hwnd, IDC_CURRENT, currenttemp, false);
			if (currenttemp >= totaltemp) {
				EnableWindow(GetDlgItem(hwnd, IDC_REG), false);
				MessageBox(NULL, UNAVAILABLE, COMPANY_NAME,
						MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1);
				AllEnable(hwnd, FALSE);
			}
//			char log[40];
//			char filepath[1024];
//			TCHAR exePath[MAX_PATH];
//			GetModuleFileName(NULL, exePath, MAX_PATH);
//			(strrchr(exePath, '\\'))[0] = 0;
//			sprintf(filepath, "%s\\machcode.txt", exePath);
//			FILE* pfile = fopen(filepath, "a+");
//			sprintf(log, "%s;%s\r\n", machcodes, registercode);
//			fwrite(log, strlen(log), 1, pfile);
//			fclose(pfile);
			memset(machcodes, 0x00, 20);

			break;
		}
		return 0;
	case WM_CLOSE:
		com_close(hComm);
		EndDialog(hwnd, TRUE);
		return 0;
	}
	return 0;
}

void* registermach::OpenCom(COMPARAM *parm) {
	parm->bByteSize = 8;
//	parm->bComId = 19;
	parm->bParity = NOPARITY;
	parm->bStopBits = ONESTOPBIT;
	parm->dwBaudRate = 4800;
	parm->fBinary = 1;

//	parm.bByteSize = 8;
//	parm.bComId = 4;
//	parm.bParity = 0;
//	parm.bStopBits = 0;
//	parm.dwBaudRate = 4800;
//	parm.fBinary = 1;
	return com_open(parm);
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int show) {

	if (DialogBox(hinst,MAKEINTRESOURCE(IDD_DIALOG1),NULL,TestProc) == -1) {

		return -1;
	}

	return 0;

}
