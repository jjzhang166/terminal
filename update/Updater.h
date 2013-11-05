/*
 * Updater.h
 *
 *  Created on: Jun 23, 2012
 *      Author: zhangbo
 */

#ifndef UPDATER_H_
#define UPDATER_H_

#include "../util/ThreadCreator.h"
#include <string>
using namespace std;

class Updater: public Runnable {
private:
	bool running;

	string application;
	string host;
	int port;
	string url;
	int interval;
	string path;

public:
	Updater();

	virtual ~Updater();

	void SetPort(int port);
	void SetUrl(const string& url);
	void SetHost(const string& host);
	void SetInterval(int interval);
	void SetPath(const string& path);
	void SetApplication(const string& application);

	void Stop();

	void Run();
};

#endif /* UPDATER_H_ */
