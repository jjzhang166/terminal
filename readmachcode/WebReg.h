/*
 * WebReg.h
 *
 *  Created on: 2012-12-17
 *      Author: xia
 */

#ifndef WEBREG_H_
#define WEBREG_H_
#include "../util/ThreadCreator.h"
class WebReg: public Runnable {
public:
	WebReg();
	virtual ~WebReg();
	void Run();
};

#endif /* WEBREG_H_ */
