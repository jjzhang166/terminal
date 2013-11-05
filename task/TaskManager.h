/*
 * TaskManager.h
 *
 *  Created on: 2012-9-8
 *      Author: zhangbo
 */

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "../thread/FileManager.h"
#include <string>
#include "CommonTask.h"
using namespace std;

namespace task {

class TaskManager {
private:
	string taskPath;
	string tempPath;
	FileManager* fileManager;
	RequestParameter parameter;

public:
	CRITICAL_SECTION critsec;
	CommonTask* currentTask;

	TaskManager();
	virtual ~TaskManager();

	void SetInterface(const RequestParameter& param);

	RequestParameter& GetInterface();

	void SetTaskPath(const string& taskId);

	string GetTaskPath();

	void SetTempPath(const string& tempPath);

	string GetTempPath();

	void SetFileManager(FileManager* fileManager);

	FileManager* GetFileManager();

	bool StartTask(const string& taskId);

	bool StopTask(const string& taskId);

	bool RemoveTask(const string& taskId);

	void RunTask(const string& taskId);
};

} /* namespace task */
#endif /* TASKMANAGER_H_ */
