/*
 * MediaApplication.h
 *
 *  Created on: 2012-8-29
 *      Author: zhangbo
 */

#ifndef MEDIAAPPLICATION_H_
#define MEDIAAPPLICATION_H_

#include "../util/Application.h"
#include "../util/CommandListener.h"
#include "../util/NetInformation.h"
#include "../util/Logger.h"
#include "../thread/FileManager.h"
#include "DrawRate.h"

#include "../task/TaskManager.h"
using namespace task;

class MediaApplication: public Application, public CommandListener {

public:
	static NetInformation* rateInfo;
	static NetInformation* scrollInfo;
	static NetInformation* ratefrequency;
	static FileManager* fileManager;
	static TaskManager* taskManager;

	static Logger* log;

	static DrawRate* rate;
	static bool canshowrate;
	string id;
	string type;

	MediaApplication();

	virtual ~MediaApplication();

	bool Start();

	static void Main(int argc, char* argv[]);

	void Receive(const string& command);

	bool StartInitialize();

	bool StartRate();

	bool StartScroll();

	void* StartChrome();

	bool StartCommand();

};

#endif /* MEDIAAPPLICATION_H_ */
