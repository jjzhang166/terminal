/*
 * FileDownloader.cpp
 *
 *  Created on: 2012-8-28
 *      Author: zhangbo
 */

#include "FileDownloader.h"
#include "BlockDownloader.h"
#include "../util/Util.h"
#include "../util/Application.h"
#include <windows.h>
#include "../util/InfoDownloader.h"
#include <stdio.h>
#include <iostream>

FileDownloader::FileDownloader(string file) {
	this->pause = false;
	this->stop = false;
	this->position = 0;
	this->file = file;
}

void FileDownloader::SetParameter(const RequestParameter& param) {
	this->parameter = param;
}

FileDownloader::~FileDownloader() {
}

void FileDownloader::SetBufferSize(unsigned long long size) {
	this->bufferSize = size;
}

void FileDownloader::SetPosition(unsigned long position) {
	this->position = position;
}

void FileDownloader::SetTempPath(const string& dir) {
	Util::EnsureDir(dir);
	this->temppath = dir;
}

void FileDownloader::Pause(bool pause) {
	this->pause = pause;
}

void FileDownloader::Stop(bool stop) {
	this->stop = stop;
}

void FileDownloader::Run() {
	HttpRequest request;
	InfoDownloader info(4096);

	if (!HttpRequest::Create(parameter).SetListener(&info).Append("down",
			"info").GetRelease(10)) {
		return;
	}

	if (!info.Process()) {
		downsuccess = false;
		return;
	}
	if (info.Get("exist") == "false") {
		downsuccess = false;
		return;
	}

	unsigned long len = info.GetUnsignedLong("size");
	unsigned long page = Util::GetPage(len, bufferSize);

	string tempfile = temppath + "/" + parameter.GetString("fid");
	BlockDownloader downloader(bufferSize);
	downloader.SetFile(tempfile);
	request.SetRequestParameter(this->parameter);
	request.PutString("down", "file");
	request.SetListener(&downloader);
	request.SetTimeout(20 * 1000);
	downloader.Open();
	while (position < page && !downloader.Cancle()) {
		request.PutInteger("start", position);
		request.PutInteger("count", page - position);
		request.PutInteger("buffer", bufferSize);
		downloader.Clear();
		if (request.Get()) {
			downloader.ProcessData();
		}
		int l = downloader.GetPosition();
		position += l;
	}
	downloader.Close();

	if (downloader.Cancle()) {
		downsuccess = false;
		return;
	}

	if (!Util::ValidateFile(tempfile, info.GetUnsignedLong("size"),
			info.Get("md5"))) {
		downsuccess = false;
		downloader.RemoveFile();
		return;
	}

	Util::CopyFile1(tempfile, file);
	Util::DeleteFile1(tempfile.c_str());
	downsuccess = true;
}

