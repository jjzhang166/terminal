/*
 * SingleApplication.cpp
 *
 *  Created on: 2012-8-29
 *      Author: zhangbo
 */

#include "SingleApplication.h"

#include <windows.h>
#include <vector>

#include "../util/Configure.h"
#include "../util/Logger.h"
#include "../util/Util.h"

SingleApplication::SingleApplication() {
}

SingleApplication::~SingleApplication() {
	if (fileManager != NULL) {
		delete fileManager;
	}
	if (taskManager != NULL) {
		delete taskManager;
	}
}

void SingleApplication::Prepare() {
	fileManager = new FileManager();
	taskManager = new TaskManager();
	string id = Util::SystemUUID(this->GetPath());
	fileManager->SetDirectory(
			Configure::Default()->GetString("filedir", "./filedir"));
	fileManager->SetTempDirectory(
			Configure::Default()->GetString("tempdir", "./tempdir"));

	RequestParameter param(
			Configure::Default()->GetString("host", "64.112.8.10"),
			Configure::Default()->GetInteger("port", 80),
			Configure::Default()->GetString("url",
					"/bank/cn/wizool/bank/servlet/InterfaceServlet"));
	param.PutString("id", id);
	param.PutString("method", "download");
	fileManager->SetRequestParameter(param);

	taskManager->SetFileManager(fileManager);
	taskManager->SetTaskPath(
			Configure::Default()->GetString("taskdir", "./taskdir"));
	taskManager->SetTempPath(
			Configure::Default()->GetString("filedir", "./filedir"));

	RequestParameter param1(
			Configure::Default()->GetString("host", "64.112.8.10"),
			Configure::Default()->GetInteger("port", 80),
			Configure::Default()->GetString("url",
					"/bank/cn/wizool/bank/servlet/InterfaceServlet"));
	param1.PutString("id", id);
	taskManager->SetInterface(param1);
}

bool SingleApplication::Start() {

	this->Prepare();
	//string strTid =
	//		"app://open:fileId=c04d960f80e6448be29224918a5d7e35;surfix=mp4;length=130020408;";
	//string strTid = "app://29db4095-9d68-43be-af65-e746c47a0ee6";
	string strTid = this->GetArguments()[1];
	string isopen = strTid.substr(strTid.find('/') + 2, 4);
	if (isopen != "open") { //打开任务
		string sstrTid = strTid.substr(strTid.find('/') + 2, 36);
		Logger::Default()->Debug(sstrTid);
		taskManager->RunTask(sstrTid);
	} else { //打开文件
		string sstrTid = strTid.substr(strTid.find('/') + 2);
		vector<string> temp = Util::Split(sstrTid, ';');
		string surfix = (Util::Split(temp.at(1), '=')).at(1);
		string fid = (Util::Split(temp.at(0), '=')).at(1);
		Logger::Default()->Debug(surfix);
		Logger::Default()->Debug(fid);
		this->Play(surfix, fid);
	}
	return true;
}

void SingleApplication::Main(int argc, char* argv[]) {
	SingleApplication app;
	app.SetArguments(argc, argv);
	app.SetName("single");
	app.SetPath("c:\\program files\\bank\\");
	app.Run();
}

void SingleApplication::Play(string surfix, string fid) {
	string type = Util::GetType(surfix);
	string param = fileManager->DownFile(fid);
	if (!Util::FileExists(param)) {
		MessageBox(NULL,
				Configure::Default()->GetString("message.fileNotFound",
						"file not found").c_str(),
				Configure::Default()->GetString("message.applicationName",
						"wizool media application").c_str(), 0);
		return;
	}
	Util::MinWindow("Google Chrome");
	if (type.compare("picture") == 0) {
		Util::PlayPhoto(param, 1);
	} else if (type.compare("video") == 0) {
		string exe = Util::GetExeFile(type);
		string par = "/fullscreen " + param;
		if (Util::ProcessCount("kmplayer.exe") == 0) {
			Util::ShellExecuteWait(exe, par, INFINITE);
		}
	} else if (type.compare("scr") == 0) {
		Util::ShellExecuteScreenSaverWait(param, INFINITE);
	} else {
		string exe = Util::GetExeFile(type);
		string par = param;
		Util::ShellExecuteWait(exe, par, INFINITE);
	}
	Util::MaxWindow("Google Chrome");
}
