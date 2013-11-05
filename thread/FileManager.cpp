/*
 * FileManager.cpp
 *
 *  Created on: 2012-8-28
 *      Author: zhangbo
 */

#include "FileManager.h"
#include "FileDownloader.h"
#include "../util/ThreadCreator.h"
#include "../util/Util.h"
#include "../util/Logger.h"

FileManager::FileManager() {
	InitializeCriticalSection(&critsec);
}

FileManager::~FileManager() {
	DeleteCriticalSection(&critsec);
}

void FileManager::SetTempDirectory(const string& temp) {
	this->temp = temp;
}

void FileManager::SetDirectory(const string& path) {
	this->path = path;
}

void FileManager::SetRequestParameter(const RequestParameter& param) {
	this->parameter = param;
}

void FileManager::RemoveFile(const string& fid) {
	if (Util::FileExists(this->path + "/" + fid)) {
		EnterCriticalSection(&critsec);
		Util::DeleteFile1(fid.c_str());
		LeaveCriticalSection(&critsec);
	}
}

bool FileManager::ExistsFile(const string& fid) {
	return Util::FileExists(this->path + "/" + fid);
}

string FileManager::DownFile(const string& fid) {
	string file = this->path + "\\" + fid;
	if (!Util::FileExists(file)) {
		EnterCriticalSection(&critsec);
		if (!Util::FileExists(file)) {
			Logger::Default()->Debug("File Downloading: " + file);
			FileDownloader* filedown = new FileDownloader(file);

			RequestParameter rp = parameter;
			rp.Put("fid", fid);
			filedown->SetParameter(rp);

			filedown->SetBufferSize(1024 * 1024 * 1);
			filedown->SetTempPath(temp);

			HANDLE h = ThreadCreator::StartThread(filedown);
			WaitForSingleObject(h, INFINITE);
			if (!Util::FileExists(file)) {
				Logger::Default()->Error("File Download Cancled: " + file);
			} else {
				Logger::Default()->Debug("File Downloaded: " + file);
			}
		}
		LeaveCriticalSection(&critsec);
	}
	return file;
}

void FileManager::DownFile(vector<string>& fids) {
	vector<string>::iterator it;
	for (it = fids.begin(); it != fids.end(); it++) {
		this->DownFile(*it);
	}
}
