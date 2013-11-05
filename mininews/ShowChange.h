/*
 * ShowChange.h
 *
 *  Created on: 2013-1-6
 *      Author: xia
 */

#ifndef SHOWCHANGE_H_
#define SHOWCHANGE_H_
#include "NewsWindow.h"
#include "../util/Runnable.h"
#include <string.h>
using namespace std;

class ShowChange: public Runnable {
private:
	bool running;
	char * details;
public:
	ShowChange();
	virtual ~ShowChange();
	BOOL InitNews();
	void Run();
	void Stop();
	string GetClipText();

};

#endif /* SHOWCHANGE_H_ */
