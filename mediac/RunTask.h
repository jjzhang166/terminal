/*
 * RunTask.h
 *
 *  Created on: 2012-8-6
 *      Author: Administrator
 */

#ifndef RUNTASK_H_
#define RUNTASK_H_

#include <windows.h>
#include "../util/Runnable.h"
#include "../util/ThreadCreator.h"
#include <vector>
#include <string>
using namespace std;

class RunTask: public Runnable {
private:
	string tid;
	bool running;

public:
	RunTask(string tid);
	virtual ~RunTask(void);

	void Run();

	void Stop();

	void UpdateProgram();
	void UpdateRate();
	void ScreenSave();
	void StartTempTask(const string& taskId);
	void ListTask();
	void ListFile();
	void ExaminationBrowser();
	void ExaminationProgram();
	void Shutdown();
	void Restart();

	void StartTask(const string& tid);
	void KillTask(const string& tid);

	void RemoveTask(const string& tid);
	void RemoveFile(const string& tid);

	void ExecuteCommand(const string& taskId);
	void UploadFile(const string& taskId);
};

#endif /* RUNTASK_H_ */
