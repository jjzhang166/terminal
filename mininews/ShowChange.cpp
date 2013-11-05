/*
 * ShowChange.cpp
 *
 *  Created on: 2013-1-9
 *      Author: xia
 */

#include "ShowChange.h"
#include "message.h"
#include "../util/ThreadCreator.h"

char oldtext[1024];
ShowChange::ShowChange() {
	// TODO Auto-generated constructor stub
	this->running = false;

}

ShowChange::~ShowChange() {
	// TODO Auto-generated destructor stub
}

string ShowChange::GetClipText() {
	string text = "";
	if (IsClipboardFormatAvailable(CF_TEXT)) {
		if (OpenClipboard(NULL)) {
			HANDLE hData = GetClipboardData(CF_TEXT);
			text = (char*) GlobalLock(hData);
			GlobalUnlock(hData);
			CloseClipboard();
		}
	}
	return text;
}
void ShowChange::Run() {
	running = true;
	string temp;
	temp = this->GetClipText();
	memset(oldtext, 0x00, 1024);
	if (temp.length() > 1020) {
		memcpy(oldtext, temp.c_str(), 1020);
	} else {
		strcpy(oldtext, temp.c_str());
	}
	while (running) {
		Sleep(1000);
		temp = "";
		temp = this->GetClipText();
		if (strcmp(oldtext, temp.c_str()) != 0) {
			if (temp == "") {
				MessageBoxA(NULL, EMPTY, COMPANY_NAME, MB_OK);
			}
			if (temp.length() > 1020) {
				memcpy(oldtext, temp.c_str(), 1020);
			} else {
				strcpy(oldtext, temp.c_str());
			}
		}

	}
}

void ShowChange::Stop() {
	running = false;
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {

//	HANDLE hMutex = CreateMutex(NULL, FALSE, "monitor");
//	if (GetLastError() == ERROR_ALREADY_EXISTS) {
//		MessageBoxA(NULL, HAVEONE, COMPANY_NAME, MB_OK);
//		CloseHandle(hMutex);
//		return 0;
//	} else {
	MessageBoxA(NULL, HAVEONE, COMPANY_NAME, MB_OK);
	ShowChange* monitor = new ShowChange();
	monitor->Run();
//	}
//	CloseHandle(hMutex);
	return 0;
}
//	void* handle =ThreadCreator::StartThread(monitor);
//	WaitForSingleObject(hand
