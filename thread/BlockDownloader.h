/*
 * BlockDownloader.h
 *
 *  Created on: 2012-11-21
 *      Author: zhangbo
 */

#ifndef BLOCKDOWNLOADER_H_
#define BLOCKDOWNLOADER_H_

#include "../util/DataBuffer.h"
#include <string>
using namespace std;

class BlockDownloader: public DataBuffer {
public:
	BlockDownloader(unsigned long size);
	virtual ~BlockDownloader();

	void SetFile(string file);
	int ProcessData();
	bool BufferProcess();
	void Open();
	void Close();
	void RemoveFile();

	bool Cancle();
	int GetPosition();

private:
	ofstream* pFile;
	string file;
	int position;
	bool md5;
	bool cancle;
};

#endif /* BLOCKDOWNLOADER_H_ */
