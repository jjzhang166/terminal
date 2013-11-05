/*
 * MediaApplication.cpp
 *
 *  Created on: 2012-8-29
 *      Author: zhangbo
 */

#include "MediaApplication.h"

#include "../util/Util.h"
#include "Config.h"
#include "ApplicationUtil.h"
#include "../util/CommandReceiver.h"
#include "../util/ThreadCreator.h"
#include "../util/Logger.h"
#include "Scrolling.h"
#include "RunTask.h"
#include "ApplicationUtil.h"
#include "StatusReport.h"

NetInformation* MediaApplication::rateInfo = NULL;
NetInformation* MediaApplication::scrollInfo = NULL;
NetInformation* MediaApplication::ratefrequency = NULL;
FileManager* MediaApplication::fileManager = NULL;
Logger* MediaApplication::log = NULL;

TaskManager* MediaApplication::taskManager = NULL;

DrawRate* MediaApplication::rate = NULL;
bool MediaApplication::canshowrate = true;
MediaApplication::MediaApplication() {

}

MediaApplication::~MediaApplication() {
	if (fileManager != NULL) {
		delete fileManager;
		fileManager = NULL;
	}
}

bool MediaApplication::StartRate() {
	rateInfo->Update();
	ratefrequency->Update();

	rate = new DrawRate("rate");
	ThreadCreator::StartThread(rate);

	return true;
}

bool MediaApplication::StartScroll() {
	this->scrollInfo->Update();
	Scrolling* scrolling = new Scrolling();
	ThreadCreator::StartThread(scrolling);
	return true;
}

bool MediaApplication::StartCommand() {
	RequestParameter param = ApplicationUtil::InterfaceParameter();
	param.SetTimeout(30 * 1000);
	param.PutString("method", "getTask");
	CommandReceiver* receiver = new CommandReceiver(param);
	receiver->SetListener(this);
	ThreadCreator::StartThread(receiver);
	return true;
}

void* MediaApplication::StartChrome() {
	string exe = Config::GetExeFile("chrome");
	string par(
			"--kiosk " + Config::GetRegisterUrl()
					+ ApplicationUtil::SystemUUID()); //
	Util::ExtWindow("Google Chrome");
	return Util::ShellExecuteNoWait(exe, par);
}

bool MediaApplication::StartInitialize() {
	Util::ImportRegisterFile(this->GetPath() + "reg.reg");

	string localfile = this->GetPath();
	Util::FilesRead(localfile.c_str());

	localfile = Config::GetStartupDir();
	Util::DeleteFolder(localfile.c_str());

	string localFile = this->GetPath() + "\\uuid.txt";
	if (Util::FileExists(localFile)) {
		Util::DeleteFile1(localFile.c_str());
	}

	string des = this->GetPath() + "update.exe";
	string src = this->GetPath() + "update1.exe";
	if (Util::FileExists(src)) {
		Util::KillProcess("update.exe");
		Sleep(2000);
		Util::DeleteFile1(des.c_str());
		Util::CopyFile1(src, des);
		Util::DeleteFile1(src.c_str());
		Util::ShellExecuteNoWait(des);
	}
	ApplicationUtil::CreateRootDir();

	RequestParameter param(Config::GetHost(), Config::GetPort(),
			Config::GetUrl());

	vector<string> v = Util::GetUUID(param, this->GetPath(), "1.1.21");
	id = v[0];
	type = v[1];
	Logger::Default()->Info(
			"mediac start: type=<" + type + "> id=<" + id + ">");

	fileManager = new FileManager();
	fileManager->SetDirectory(Config::GetFileDir());
	fileManager->SetTempDirectory(Config::GetDownTmpFile());
	param.PutString("id", id);
	param.PutString("method", "download");
	fileManager->SetRequestParameter(param);

	taskManager = new TaskManager();
	taskManager->SetFileManager(fileManager);
	taskManager->SetTaskPath(Config::GetRootDir());
	taskManager->SetTempPath(Config::GetTempDir());
	taskManager->SetInterface(ApplicationUtil::InterfaceParameter());

	RequestParameter param1(Config::GetHost(), Config::GetPort(),
			Config::GetUrl());
	param1.PutString("method", "active");
	param1.PutString("id", id);
	ThreadCreator::StartThread(new StatusReport(param1, 30 * 1000));

	return true;
}

bool MediaApplication::Start() {
	Logger::Default()->SetLevel(Logger::LOG_INFO);

	//初始化环境
	StartInitialize();

	if (type == "media") {
		rateInfo = new NetInformation(Config::GetHost(), Config::GetPort(),
				Config::GetUrl() + "?method=getRate&id=" + id, 4096);
		scrollInfo = new NetInformation(Config::GetHost(), Config::GetPort(),
				Config::GetUrl() + "?method=getRoll&id=" + id, 4096);
		ratefrequency = new NetInformation(Config::GetHost(), Config::GetPort(),
				Config::GetUrl() + "?method=getFrequency&id=" + id, 4096);

		//播放利率牌
		StartRate();

		//播放滚动条
		StartScroll();
	}

	//开始执行任务
	StartCommand();

	//打开谷歌浏览器
	void* h = StartChrome();

	//列出机器上的文件，传回给服务器，服务器会通知删除所有过期的文件
	this->Receive("S:f6f2a76c-bf34-4cc7-adba-8d2ab99d0fba");

	Sleep(5000);
	keybd_event(VK_F5, 0, 0, 0);
	keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);

	//列出机器上的任务，传回给服务器，服务器会通知删除所有过期的任务
	this->Receive("S:f6f2a76c-bf34-4cc7-adba-8d2ab99d0fbb");

	WaitForSingleObject(h, INFINITE);

	return true;
}

void MediaApplication::Main(int argc, char* argv[]) {
	MediaApplication app;
	app.SetArguments(argc, argv);
	app.SetName("mediac");
	app.SetPath("C:\\Program Files\\bank\\");
	app.Run();
}

void MediaApplication::Receive(const string& command) {
	if (log == NULL) {
		log = new Logger(this->GetPath() + "command.log");
		log->SetLevel(Logger::LOG_DEBUG);
	}
	log->Debug("CMD:" + command);
	ThreadCreator::StartThread(new RunTask(command));
}
