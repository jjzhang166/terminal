/*
 * WebReg.cpp
 *
 *  Created on: 2012-12-17
 *      Author: xia
 */

#include "WebReg.h"
#include <windows.h>
#include "../util/Util.h"
#include "../util/ThreadCreator.h"
#include <stdio.h>
#include "path.h"
#include <string.h>
using namespace std;

WebReg::WebReg() {
	// TODO Auto-generated constructor stub

}

WebReg::~WebReg() {
	// TODO Auto-generated destructor stub
}

void WebReg::Run() {
	char filepath[1024];
	TCHAR exePath[MAX_PATH];
	char machfile[1024];
	GetModuleFileName(NULL, exePath, MAX_PATH);
	(strrchr(exePath, '\\'))[0] = 0;
	sprintf(filepath, "%s\\machcode", exePath);

	string machcodes = Util::FindFiles(filepath, true);
	vector<string> temp = Util::Split(machcodes, ';');
	for (int i = 0; i < temp.size(); i++) {
		sprintf(machfile, "%s\\%s", filepath, temp[i].c_str());

		RequestParameter param1("58.58.25.175", 8082,
				"/wzms/cn/wizool/wzms/servlet/MainServlet");
		param1.PutString("method", "returnRegisterNum");
		param1.PutString("cid", "xinhuayou");
		param1.PutString("nid", temp[i]);

		if (!Util::Download(param1, machfile)) {
			MessageBoxA(NULL, failed, "wizool", MB_OK);
		};
	}
	MessageBoxA(NULL, success, "wizool", MB_OK);
}

//int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
//
//	if (Util::ProcessCount("webreg.exe") <= 1) {
//		WebReg* pWebReg = new WebReg();
//
//		void* handle = ThreadCreator::StartThread(pWebReg);
//		WaitForSingleObject(handle, INFINITE);
//	} else
//		MessageBoxA(NULL, haveone, "wizool", MB_OK);
//	return 0;
//}

