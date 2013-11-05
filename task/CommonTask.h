/*
 * CommonTask.h
 *
 *  Created on: 2012-9-8
 *      Author: zhangbo
 */

#ifndef COMMONTASK_H_
#define COMMONTASK_H_

#include "../thread/FileManager.h"
#include "../util/InfoDownloader.h"
#include "../util/RequestParameter.h"
#include <string>
using namespace std;

namespace task {

class CommonTask {
private:
	string taskId;
	InfoDownloader information;
	string parent;
	FileManager* fileManager;
	RequestParameter* parameter;
	vector<string> files;
	string type;
	unsigned long age;
	string names;
	string tempPath;
	bool download;
	bool validate;

	bool HasFiles();
	bool Download();
	bool EnumDirectory();

	void* Play(const vector<string>& files, const string& type = "video");
public:
	CommonTask(RequestParameter* parameter, const string& taskId,
			const string& parent, FileManager* fileManager);
	CommonTask(RequestParameter* parameter, const string& taskId,
			const string& dir);

	virtual ~CommonTask();

	bool Prepare();
	bool DownloadFiles();
	bool CopyFiles();

	bool isDownload();
	bool isValidate();

	void SendTaskStatus(const string& status);

	unsigned long GetAge();
	string GetTaskId();
	string GetPath();

	void* RunBegin(bool showList);
	void RunEnd();

	void Stop();

	void Remove();
};

} /* namespace task */
#endif /* COMMONTASK_H_ */
