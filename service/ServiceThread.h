/*
 * service.h
 *
 *  Created on: 2013-1-22
 *      Author: xia
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include "Runnable.h"

class ServiceThread: public Runnable {
public:
	ServiceThread();
	virtual ~ServiceThread();

	void Run();
};

#endif /* SERVICE_H_ */
