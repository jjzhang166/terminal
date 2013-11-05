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
#include <stdio.h>

using namespace std;
char machcodes[20];
char registercode[20];
int first = 0;
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

static int CALLBACK TestProc(HWND hwnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_CPUSTR));
		SendMessageA(GetDlgItem(hwnd, IDC_CPUSTR), EM_LIMITTEXT, 4, 0);
		SendMessageA(GetDlgItem(hwnd, IDC_DISKSTR), EM_LIMITTEXT, 4, 0);
		SendMessageA(GetDlgItem(hwnd, IDC_MACSTR), EM_LIMITTEXT, 4, 0);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_LOCAL: {
			char localRegistCod[20];
			string temp;

			SetDlgItemTextA(hwnd, IDC_REGIESTCODE, "");
			memset(localRegistCod, 0x00, 20);
			temp = "";
			temp = Util::GenerateMach(localRegistCod, 1);
			SetDlgItemTextA(hwnd, IDC_CPUSTR, temp.c_str());
			temp = "";
			temp = Util::GenerateMach(localRegistCod, 2);
			SetDlgItemTextA(hwnd, IDC_DISKSTR, temp.c_str());
			temp = "";
			temp = Util::GenerateMach(localRegistCod, 3);
			SetDlgItemTextA(hwnd, IDC_MACSTR, temp.c_str());
			break;
		}
		case IDC_CLEAR:
			SetDlgItemTextA(hwnd, IDC_CPUSTR, "");
			SetDlgItemTextA(hwnd, IDC_DISKSTR, "");
			SetDlgItemTextA(hwnd, IDC_MACSTR, "");
			SetDlgItemTextA(hwnd, IDC_REGIESTCODE, "");
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
			registermach::GetMachCode(hwnd, IDC_CPUSTR, machcodes);
			machcodes[4] = '-';
			registermach::GetMachCode(hwnd, IDC_DISKSTR, machcodes);
			machcodes[9] = '-';
			registermach::GetMachCode(hwnd, IDC_MACSTR, machcodes);
			machcodes[14] = '0';
			machcodes[15] = '0';
			char registerkey[1024];
			memset(registerkey, 0x00, 1024);
			//微卓
//			encrypt_3des(":\"fI`gQM", "w?;,;n=b", "-[d^8g2B",
//					(unsigned char *) machcodes, (unsigned char *) registerkey);

			//汇金
			encrypt_3des("f<w$/_db", "slOx'4\\#", "*MiYzXQ#",
					(unsigned char *) machcodes, (unsigned char *) registerkey);

			int hash = Util::RSHash(registerkey);
			sprintf(registercode, "%06d", hash % 1000000);
			SetDlgItemTextA(hwnd, IDC_REGIESTCODE, registercode);
			char log[40];
			char filepath[1024];
			TCHAR exePath[MAX_PATH];
			GetModuleFileName(NULL, exePath, MAX_PATH);
			(strrchr(exePath, '\\'))[0] = 0;
			sprintf(filepath, "%s\\machcode.txt", exePath);
			FILE* pfile = fopen(filepath, "a+");
			sprintf(log, "%s;%s\r\n", machcodes, registercode);
			fwrite(log, strlen(log), 1, pfile);
			fclose(pfile);
			memset(machcodes, 0x00, 20);

			break;
		}
		return 0;
	case WM_CLOSE:
		EndDialog(hwnd, TRUE);
		return 0;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int show) {
	if (DialogBox(hinst,MAKEINTRESOURCE(IDD_DIALOG1),NULL,TestProc) == -1) {
		return 0;
	}
}
