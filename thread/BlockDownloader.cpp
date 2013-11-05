/*
 * BlockDownloader.cpp
 *
 *  Created on: 2012-11-21
 *      Author: zhangbo
 */

#include "BlockDownloader.h"
#include <string.h>
#include "../util/Util.h"
#include "../util/Logger.h"

#define ERROR_RETURN 1000

#include <fstream>
#include <iostream>

BlockDownloader::BlockDownloader(unsigned long size) :
		DataBuffer(size + 25) {
	position = 0;
	md5 = true;
	cancle = false;
}

BlockDownloader::~BlockDownloader() {
}

void BlockDownloader::SetFile(string file) {
	this->file = file;
}

bool BlockDownloader::BufferProcess() {
	bool ret = (ProcessData() == 0);
	return ret;
}

int BlockDownloader::ProcessData() {
	unsigned char *dt = (unsigned char *) buffer;
	unsigned long long curlen;

	if (*buffer == 'A') {
		cancle = false;
		return ERROR_RETURN;
	}

	if (this->GetLength() < 9) {
		return ERROR_RETURN;
	}

	if (*buffer == 'S') {
		if (this->GetLength() < 25) {
			return ERROR_RETURN;
		}
		curlen = Util::ToUnsignedLongLong(buffer + 1);
		if (this->GetLength() != curlen + 25) {
			return ERROR_RETURN;
		}
	} else {
		return ERROR_RETURN;
	}

	unsigned char *curdata;
	curdata = dt + 9;

	if (md5) {
		unsigned char newmd5[16];
		Util::MD5(curdata, curlen, newmd5);
		if (memcmp(curdata + curlen, newmd5, 16) != 0) {
			return ERROR_RETURN;
		}
	}

	pFile->write((const char*) curdata, curlen);
	position++;

	return 0;
}

void BlockDownloader::RemoveFile() {
	unlink(this->file.c_str());
}

void BlockDownloader::Open() {
	unlink(this->file.c_str());
	pFile = new ofstream(file.c_str(), ios::binary | ios::app);
}

bool BlockDownloader::Cancle() {
	return cancle;
}

void BlockDownloader::Close() {
	pFile->close();
	delete pFile;
}

int BlockDownloader::GetPosition() {
	int pos = this->position;
	this->position = 0;
	return pos;
}
