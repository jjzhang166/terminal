/*
 * ApplicationUtil.cpp
 *
 *  Created on: 2012-8-26
 *      Author: zhangbo
 */

#include "ApplicationUtil.h"

#include "../util/Util.h"
#include "../util/HttpRequest.h"

#include "Config.h"
#include "../util/Logger.h"
#include "DrawRate.h"
#include "MediaApplication.h"

#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 4096

ApplicationUtil::ApplicationUtil() {

}

ApplicationUtil::~ApplicationUtil() {

}

void ApplicationUtil::MouseEvent() {
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); //按下
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //弹起
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); //按下
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //弹起
}

string ApplicationUtil::GetValue(string files, char chr, int n) {
	vector<string> tempvalue = Util::Split(files, chr);
	return tempvalue.at(n);
}

string ApplicationUtil::SystemUUID() {
	string uuidFile = Config::InstallPath + "uuid.txt";
	return Util::ReadFile(uuidFile, 40);
}

bool ApplicationUtil::SendMessages(string message) {
	HttpRequest req;
	req.SetHost(Config::GetHost());
	req.SetPort(Config::GetPort());
	req.SetUrl(Config::GetUrl() + "?" + message);
	return req.Get(10);
}

bool ApplicationUtil::PostMessages(Properties& data) {
	return ApplicationUtil::InterfaceRequest().SetParameters(data).PostRelease(
			10);
}

void ApplicationUtil::PlayPhoto(string filePath, unsigned int age) {
	string systemDir = Config::GetSystemDir();
	string runDllDir = systemDir + "\\rundll32.exe";
	string param = systemDir + "\\shimgvw.dll,ImageView_Fullscreen " + filePath;

	HANDLE h = Util::ShellExecuteNoWait(runDllDir, param);
	Sleep(1000);
	keybd_event(VK_F11, 0, 0, 0);
	keybd_event(VK_F11, 0, KEYEVENTF_KEYUP, 0);
	WaitForSingleObject(h, age);
}

bool ApplicationUtil::CreateRootDir() {
	Util::EnsureDir(Config::GetRootDir());
	Util::EnsureDir(Config::GetFileDir());
	return true;
}

HttpRequest& ApplicationUtil::InterfaceRequest() {
	HttpRequest* p = new HttpRequest();
	p->SetHost(Config::GetHost());
	p->SetPort(Config::GetPort());
	p->SetUrl(Config::GetUrl());
	return *p;
}

HttpRequest& ApplicationUtil::InterfaceRequest(const string& method) {
	HttpRequest* p = new HttpRequest();
	p->SetHost(Config::GetHost());
	p->SetPort(Config::GetPort());
	p->SetUrl(Config::GetUrl());
	return p->Append("method", method);
}

RequestParameter ApplicationUtil::InterfaceParameter() {
	RequestParameter param;
	param.SetHost(Config::GetHost());
	param.SetPort(Config::GetPort());
	param.SetUrl(Config::GetUrl());
	param.PutString("id", ApplicationUtil::SystemUUID());
	return param;
}

//void ApplicationUtil::ShowList(string filenames) {
//	MediaApplication::canshowrate = false;
//	if (NULL != MediaApplication::rate) {
//		MediaApplication::rate->SetVisible(false);
//	}
//	string bmpfile = Config::InstallPath + "namelist.bmp";
//	WNDCLASS listclass;
//	DrawList list("list", listclass);
//	list.Create();
//	Sleep(400);
//	list.Show();
//	list.SetTopMost(true);
//	list.SetVisible(true);
//	list.GetfileNameList(filenames);
//	list.SetBmpFile(bmpfile);
//	list.Refresh();
//	Sleep(10000);
//	list.SetVisible(false);
//	MediaApplication::canshowrate = true;
//}
