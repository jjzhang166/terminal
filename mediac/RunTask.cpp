/*
 * Runtask.cpp
 *
 *  Created on: 2012-8-6
 *      Author: Administrator
 */
#include "RunTask.h"
#include "Config.h"
#include "../util/Util.h"
#include "ApplicationUtil.h"
#include "../util/Logger.h"
#include "DrawRate.h"
#include "MediaApplication.h"
#include "../thread/FileDownloader.h"
#include "../task/CommonTask.h"
#include "../util/HttpClient.h"

RunTask::RunTask(string tid) {
	this->tid = tid;
}

RunTask::~RunTask(void) {

}

void RunTask::Run() {
	if (tid.length() < 3) {
		return;
	}

	string taskId = tid.substr(2);
	if (tid.substr(0, 2) == "S:") {
		if (taskId.compare("f6f2a76c-bf34-4cc7-adba-8d2ab99d0fb8") == 0) {
			this->Shutdown();
		} else if (taskId.compare("f6f2a76c-bf34-4cc7-adba-8d2ab99d0fb9")
				== 0) {
			this->Restart();
		} else if (taskId.compare("f6f2a76c-bf34-4cc7-adba-8d2ab99d0fba")
				== 0) {
			this->ListTask();
		} else if (taskId.compare("f6f2a76c-bf34-4cc7-adba-8d2ab99d0fbb")
				== 0) {
			this->ListFile();
		} else if (taskId.compare("f6f2a76c-bf34-4cc7-adba-8d2ab99d0fbc")
				== 0) {
			this->ExaminationBrowser();
		} else if (taskId.compare("f6f2a76c-bf34-4cc7-adba-8d2ab99d0fbd")
				== 0) {
			this->ExaminationProgram();
		} else if (taskId.compare("getConfig") == 0) {
			this->UpdateRate();
		} else if (taskId.compare("screenSave") == 0) {
			this->ScreenSave();
		} else if (taskId.compare("updateProgram") == 0) {
			this->UpdateProgram();
		} else {
			MediaApplication::taskManager->StartTask(taskId);
		}
	} else if (tid.substr(0, 2) == "K:") {
		MediaApplication::taskManager->StopTask(taskId);
	} else if (tid.substr(0, 2) == "D:") {
		MediaApplication::fileManager->DownFile(taskId);
	} else if (tid.substr(0, 2) == "C:") {
		this->ExecuteCommand(taskId);
	} else if (tid.substr(0, 2) == "U:") {
		this->UploadFile(taskId);
	} else if (tid.substr(0, 2) == "R:") {
		this->RemoveTask(taskId);
		this->RemoveFile(taskId);
	} else if (tid == "shutdown") {
		this->Shutdown();
	} else if (tid == "restart") {
		this->Restart();
	}
}

void RunTask::UpdateProgram() {
	Util::WriteFile(Config::InstallPath + "version.txt", Util::CreateGUID());
}

void RunTask::ScreenSave() {
	string file = Config::InstallPath + "screensave.bmp";
	Util::SaveScreen(file);
	HttpClient c(Config::GetHost(), Config::GetPort(), Config::GetUrl());
	c.SetFile(file);
	c.SetId(ApplicationUtil::SystemUUID());
	c.Post();
}

void RunTask::UpdateRate() {
	MediaApplication::rateInfo->Update();

	MediaApplication::ratefrequency->Update();
	MediaApplication::scrollInfo->Update();
}

void RunTask::ListTask() {
	Properties properties;
	string localfile = Config::GetRootDir();
	string tasklist = Util::FindFiles(localfile.c_str(), true);
	properties.Put("method", "taskList");
	properties.Put("id", ApplicationUtil::SystemUUID());
	properties.Put("ids", tasklist);
	ApplicationUtil::PostMessages(properties);
}

void RunTask::ListFile() {
	Properties properties;
	string localfile = Config::GetFileDir();
	string filelist = Util::FindFiles(localfile.c_str(), false);
	properties.Put("method", "fileList");
	properties.Put("id", ApplicationUtil::SystemUUID());
	properties.Put("ids", filelist);
	ApplicationUtil::PostMessages(properties);
}

void RunTask::ExaminationBrowser() {
	string exe = "C:\\Program Files\\Internet Explorer\\iexplore.exe";
	string par = Config::GetIePar();
	Util::ShellExecuteWait(exe, par);
}

void RunTask::ExaminationProgram() {
	string exedir = Config::GetRccExamDir();
	SetCurrentDirectory(exedir.c_str());
	string exe = Config::GetRccExamExe();
	string par = "";
	HANDLE hProcess = Util::ShellExecuteNoWait(exe, par);
	string title = Config::GetWindowTitle("examtitle");
	Sleep(500);
	Util::TopWindow(title);
	Sleep(500);
	WaitForSingleObject(hProcess, INFINITE);
}

void RunTask::Shutdown() {
	Util::ShutDown();
}

void RunTask::Restart() {
	Util::Reboot();
}

void RunTask::RemoveFile(const string& fid) {
	string localFile = Config::GetFileDir() + "\\" + fid;
	if (Util::FileExists(localFile)) {
		Util::DeleteFile1(localFile.c_str());
		ApplicationUtil::SendMessages(
				"method=removeFile&id=" + ApplicationUtil::SystemUUID()
						+ "&tid=" + fid);
	}
}

void RunTask::RemoveTask(const string& taskId) {
	string localFile = Config::GetRootDir() + "\\" + taskId;
	if (Util::FileExists(localFile)) {
		Util::DeleteFolder(localFile.c_str());
		RemoveDirectory(localFile.c_str());
	}
	ApplicationUtil::SendMessages(
			"method=removeTask&id=" + ApplicationUtil::SystemUUID() + "&tid="
					+ taskId);
}

void RunTask::Stop() {
	this->running = false;
}

void RunTask::ExecuteCommand(const string& taskId) {
	string file = Config::InstallPath + "command.txt";
	Util::ExecuteCommand(taskId, Config::InstallPath,
			Config::InstallPath + "command.txt");
	HttpClient c(Config::GetHost(), Config::GetPort(), Config::GetUrl());
	c.SetFile(file);
	c.SetId(ApplicationUtil::SystemUUID());
	c.Post();
}

void RunTask::UploadFile(const string& taskId) {
	HttpClient c(Config::GetHost(), Config::GetPort(), Config::GetUrl());
	c.SetFile(taskId);
	c.SetId(ApplicationUtil::SystemUUID());
	c.Post();
}
