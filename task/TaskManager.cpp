/*
 * TaskManager.cpp
 *
 *  Created on: 2012-9-8
 *      Author: zhangbo
 */

#include "TaskManager.h"
#include "../util/Logger.h"
#include <time.h>

namespace task {

TaskManager::TaskManager() {
	this->currentTask = NULL;
	InitializeCriticalSection(&critsec);
}

TaskManager::~TaskManager() {
	DeleteCriticalSection(&critsec);
}

void TaskManager::SetTaskPath(const string& taskPath) {
	this->taskPath = taskPath;
}

string TaskManager::GetTaskPath() {
	return this->taskPath;
}

void TaskManager::SetTempPath(const string& tempPath) {
	this->tempPath = tempPath;
}

string TaskManager::GetTempPath() {
	return this->tempPath;
}

void TaskManager::SetFileManager(FileManager* fileManager) {
	this->fileManager = fileManager;
}

FileManager* TaskManager::GetFileManager() {
	return this->fileManager;
}

typedef struct _TimeOut {
	CommonTask* task;
	TaskManager* manager;
	unsigned long sleep; //首先睡眠时间
	unsigned long random; //随机等待时间
	void* waitevent; //等待事件
} TimeOut;

DWORD WINAPI DescriptionProc(LPVOID pthread) {
	TimeOut* to = (TimeOut*) pthread;
	CommonTask* task = to->task;
	Logger::Default()->Info(
			"Prepare download description of task: " + task->GetTaskId());
	Sleep(to->sleep + (rand() % to->random));
	Logger::Default()->Info(
			"Download description of task: " + task->GetTaskId());
	task->Prepare();
	Logger::Default()->Info(
			"Downloaded description of task: " + task->GetTaskId());
	delete to;
	return 0;
}

DWORD WINAPI TaskStartProc(LPVOID pthread) {
	TimeOut* to = (TimeOut*) pthread;
	TaskManager* thiz = to->manager;
	CommonTask* task = to->task;
	Sleep(to->sleep);
	WaitForSingleObject(to->waitevent, INFINITE);

	if (!task->isDownload()) {
		srand((unsigned) time(NULL));

		//在10-20分钟的随机时间开始下载文件
		Logger::Default()->Info(
				"Prepare download files of task: " + task->GetTaskId());
		Sleep(20 * 1000 + (rand() % (20 * 1000)));
		Logger::Default()->Info(
				"Start download files of task: " + task->GetTaskId());
		task->SendTaskStatus("down");
		task->DownloadFiles();
		Logger::Default()->Info(
				"Downloaded files of task: " + task->GetTaskId());
	}

	//启动任务
	if (task->isDownload() && task->CopyFiles()) {
		EnterCriticalSection(&thiz->critsec);
		Logger::Default()->Info("Start task: " + task->GetTaskId());
		if (thiz->currentTask != NULL) {
			thiz->currentTask->Stop();
			Sleep(1500);
			thiz->currentTask = NULL;
		}
		void* h = task->RunBegin(true);
		thiz->currentTask = task;
		LeaveCriticalSection(&thiz->critsec);
		WaitForSingleObject(h, task->GetAge());
		EnterCriticalSection(&thiz->critsec);
		if (thiz->currentTask == NULL) {
			//kill task
			task->RunEnd();
		} else if (thiz->currentTask == task) {
			//timeout
			task->Stop();
			task->RunEnd();
			thiz->currentTask = NULL;
		}
		Logger::Default()->Info("End task: " + task->GetTaskId());
		LeaveCriticalSection(&thiz->critsec);
	} else {
		//无法完成任务，报告错误
		Logger::Default()->Error("Cannot Run Task:" + task->GetTaskId());
	}
	delete task;
	delete to;
	return 0;
}

bool TaskManager::StartTask(const string& taskId) {
	CommonTask* task = new CommonTask(&this->parameter, taskId,
			this->GetTaskPath(), this->fileManager);

	srand((unsigned) time(NULL));

	TimeOut* pDesc = new TimeOut;
	pDesc->task = task;
	pDesc->sleep = 5 * 1000; //服务器下发任务的时间
	pDesc->random = 20 * 1000; //所有的机器访问服务器下载描述的时间

	TimeOut* pTask = new TimeOut;
	pTask->waitevent = CreateThread(NULL, 0, DescriptionProc, (LPVOID) pDesc, 0,
			0);
	pTask->manager = this;
	pTask->task = task;
	pTask->sleep = 60 * 1000; //提前下发任务的时间（1分钟）
	pTask->random = 0;
	CreateThread(NULL, 0, TaskStartProc, (LPVOID) pTask, 0, 0);

	return false;
}

void TaskManager::RunTask(const string& taskId) {
	CommonTask* task = new CommonTask(&this->parameter, taskId,
			this->GetTaskPath(), this->fileManager);
	task->Prepare();
	if (task->isValidate()) {
		if (!task->isDownload()) {
			task->DownloadFiles();
		}

		if (task->isDownload() && task->CopyFiles()) {
			void* h = task->RunBegin(false);
			WaitForSingleObject(h, task->GetAge());
			task->Stop();
			task->RunEnd();
		}
	}
	delete task;
}

bool TaskManager::StopTask(const string& taskId) {
	EnterCriticalSection(&critsec);
	bool ret = false;
	if (currentTask != NULL && currentTask->GetTaskId() == taskId) {
		currentTask->Stop();
		Sleep(1500);
		currentTask = NULL;
		ret = true;
	}
	LeaveCriticalSection(&critsec);
	return ret;
}

bool TaskManager::RemoveTask(const string& taskId) {
	return false;
}

void TaskManager::SetInterface(const RequestParameter& param) {
	this->parameter = param;
}

RequestParameter& TaskManager::GetInterface() {
	return this->parameter;
}

} /* namespace task */
