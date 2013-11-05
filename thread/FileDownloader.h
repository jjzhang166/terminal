/*
 * FileDownloader.h
 *
 *  Created on: 2012-8-28
 *      Author: zhangbo
 */

#ifndef FILEDOWNLOADER_H_
#define FILEDOWNLOADER_H_

#include "../util/Runnable.h"
#include "../util/HttpRequest.h"
#include <string>
using namespace std;

class FileDownloader: public Runnable {

private:
	RequestParameter parameter;
	string fid;

	bool pause;
	bool stop;
	bool downsuccess;

	string file;
	string temppath;
	unsigned long bufferSize;
	unsigned long position;

public:
	FileDownloader(string filepath);
	virtual ~FileDownloader();

	void SetParameter(const RequestParameter& param);
	void SetBufferSize(unsigned long long BUFFER_SIZE);
	void SetPosition(unsigned long cur);
	void SetTempPath(const string& dir);

	void Pause(bool pause);
	void Stop(bool stop);

	void Run();
	void Run1();

};

#endif /* FILEDOWNLOADER_H_ */
