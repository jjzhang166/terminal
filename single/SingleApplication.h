/*
 * SingleApplication.h
 *
 *  Created on: 2012-8-29
 *      Author: zhangbo
 */

#ifndef SINGLEAPPLICATION_H_
#define SINGLEAPPLICATION_H_

#include "../util/Application.h"
#include "../thread/FileManager.h"
#include "../task/TaskManager.h"

using namespace task;

class SingleApplication: public Application {
private:
	FileManager* fileManager;
	TaskManager* taskManager;

public:
	SingleApplication();
	virtual ~SingleApplication();

	void Prepare();

	bool Start();

	void Play(string type, string fid);

	static void Main(int argc, char* argv[]);
};

#endif /* SINGLEAPPLICATION_H_ */
