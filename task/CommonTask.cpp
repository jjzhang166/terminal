/*
 * CommonTask.cpp
 *
 *  Created on: 2012-9-8
 *      Author: zhangbo
 */

#include "CommonTask.h"
#include "../util/Util.h"
#include "../util/Configure.h"
#include "../util/HttpRequest.h"
#include "../util/Logger.h"
#include "DrawList.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
using namespace std;

namespace task {

CommonTask::CommonTask(RequestParameter* parameter, const string& taskId,
		const string& parent, FileManager* fileManager) :
		information(10 * 1024) {
	this->type = "";
	this->taskId = taskId;
	this->parent = parent;
	this->fileManager = fileManager;
	this->parameter = parameter;
	this->names = "";
	age = 0xffffffff;
	this->validate = false;
	this->download = false;
}

CommonTask::CommonTask(RequestParameter* parameter, const string& taskId,
		const string& dir) :
		information(0) {
	this->type = "";
	this->taskId = taskId;
	age = 0xffffffff;
	this->names = "";
	this->parameter = parameter;
	this->tempPath = dir;
	this->validate = false;
	this->download = false;
}

CommonTask::~CommonTask() {
}

string CommonTask::GetTaskId() {
	return this->taskId;
}

string CommonTask::GetPath() {
	return this->parent + "\\" + this->taskId;
}

unsigned long CommonTask::GetAge() {
	return age;
}

void* CommonTask::RunBegin(bool showList) {
	this->SendTaskStatus("run");
	Util::MinWindow("Google Chrome");
	if (showList) {
		DrawList::ShowList(Util::Split(this->names, ';'), 10);
	}
	return this->Play(this->files, this->type);
}

void CommonTask::RunEnd() {
	this->SendTaskStatus("stop");
	Util::MaxWindow("Google Chrome");
}

void* CommonTask::Play(const vector<string>& files, const string& type) {
	string param = Util::Join(files, ' ');
	void* h = NULL;

	Logger::Default()->Info(
			"Task Start: Task=" + this->taskId + " Type=" + type + " Files="
					+ Util::Join(files, ' '));

	if (type == "video") {
		string exe = Util::GetExeFile(type);
		Util::KillProcess(Util::GetProcess(type));
		string par = "/fullscreen " + param;
		h = Util::ShellExecuteNoWait(exe, par);
//	} else if (type == "picture") {
//		ApplicationUtil::PlayPhoto(this->GetPath() + "\\001"+type, 1);
//		h = ApplicationUtil::PlayPhoto(this->GetPath());
//	} else if (type == "scr") {
//		h = Util::ShellExecuteScreenSaverNoWait(param);
	} else {
		string exe = Util::GetExeFile(type);
		Util::KillProcess(Util::GetProcess(type));
		h = Util::ShellExecuteNoWait(exe, param);
	}
	return h;
}

/**
 * status: prepare down run stop
 */
void CommonTask::SendTaskStatus(const string& status) {
	HttpRequest::Create(*this->parameter).Append("method", "taskStatus").Append(
			"tid", this->taskId).Append("status", status).GetRelease(10);
}

void CommonTask::Stop() {
	if (type.compare("scr") == 0) {
		keybd_event(115, 0, 0, 0);
		keybd_event(115, 0, KEYEVENTF_KEYUP, 0);
	} else {
		Util::KillProcess(Util::GetProcess(type));
	}
}

void CommonTask::Remove() {
	Util::DeleteFolder(this->GetPath().c_str());
}

bool CommonTask::Prepare() {
	Util::EnsureDir(this->GetPath());
	HttpRequest::Create(*this->parameter).Append("method", "getDesc").SetListener(
			&information).Append("tid", this->taskId).GetRelease(10);
	if (!information.Process()) {
		this->validate = false;
		return this->validate;
	}

	if (information.GetInteger("age", 0) > 0) {
		age = information.GetInteger("age") * 1000;
	}
	names = information.Get("names", "empty");
	this->validate = true;
	this->download = this->HasFiles();
	return this->validate;
}

bool CommonTask::isDownload() {
	return this->download;
}

bool CommonTask::isValidate() {
	return this->validate;
}

bool CommonTask::HasFiles() {
	if (!this->validate) {
		return false;
	}
	vector<string> v = Util::Split(information.Get("fileName"), ';');
	vector<string>::iterator it;
	int i = 1;
	bool ret = true;
	for (it = v.begin(); it != v.end(); it++, i++) {
		vector<string> nv = Util::Split(*it, '.');
		if (nv.size() == 2) {
			string des = this->GetPath() + "\\" + Util::ToString(i) + "."
					+ nv[1];
			this->files.push_back(des);
			if (!this->fileManager->ExistsFile(nv[0])) {
				ret = false;
			}
			if (i == 1) {
				this->type = Util::GetType(nv[1]);
			} else if (this->type != Util::GetType(nv[1])) {
				ret = false;
				Logger::Default()->Error(
						"Task Type Error: Task=" + this->taskId + " Type="
								+ type + " Files=" + Util::Join(files, ' '));
			}
		}
	}
	return ret;
}

bool CommonTask::EnumDirectory() {
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind, tempPath.c_str());
	strcat(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return false;
	int i = 1;
	do {
		if (strcmp(FindFileData.cFileName, ".") != 0
				&& strcmp(FindFileData.cFileName, "..") != 0) {
			string filepath = tempPath;
			filepath += "\\";
			filepath += FindFileData.cFileName;
			files.push_back(filepath);
			names += FindFileData.cFileName;
			names += ";";
			vector<string> temp = Util::Split(FindFileData.cFileName, '.');
			if (i == 1) {
				this->type = Util::GetType(temp[1]);
			} else if (this->type != Util::GetType(temp[1])) {
				return false;
			}
			i++;
		}
	} while (::FindNextFile(hFind, &FindFileData));
	::FindClose(hFind);
	return true;
}

bool CommonTask::DownloadFiles() {
	if (!this->validate) {
		this->download = false;
		return this->download;
	}
	vector<string> v = Util::Split(information.Get("fileName"), ';');
	vector<string>::iterator it;
	int i = 1;
	for (it = v.begin(); it != v.end(); it++, i++) {
		vector<string> nv = Util::Split(*it, '.');
		if (nv.size() == 2) {
			string src = this->fileManager->DownFile(nv[0]);
			string des = this->GetPath() + "\\" + Util::ToString(i) + "."
					+ nv[1];

			if (!Util::FileExists(src)) {
				this->download = false;
				return this->download;
			}
			if (!Util::FileExists(des)) {
				Util::CopyFile1(src, des);
			}
			if (!Util::FileExists(des)) {
				this->download = false;
				return this->download;
			}
		}
	}
	this->download = true;
	return this->download;
}

bool CommonTask::CopyFiles() {
	if (!this->validate || !this->download) {
		return false;
	}
	vector<string> v = Util::Split(information.Get("fileName"), ';');
	vector<string>::iterator it;
	int i = 1;
	for (it = v.begin(); it != v.end(); it++, i++) {
		vector<string> nv = Util::Split(*it, '.');
		if (nv.size() == 2) {
			string src = this->fileManager->DownFile(nv[0]);
			string des = this->GetPath() + "\\" + Util::ToString(i) + "."
					+ nv[1];
			if (!Util::FileExists(src)) {
				return false;
			}
			if (!Util::FileExists(des)) {
				Util::CopyFile1(src, des);
			}
			if (!Util::FileExists(des)) {
				return false;
			}
			if (i == 1) {
				this->type = Util::GetType(nv[1]);
			} else if (this->type != Util::GetType(nv[1])) {
				return false;
			}
		}
	}
	return true;
}

} /* namespace task */
