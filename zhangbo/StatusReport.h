/*
 * StatusReport.h
 *
 *  Created on: 2012-6-28
 *      Author: zhangbo
 */

#ifndef STATUSREPORT_H_
#define STATUSREPORT_H_

#include "../util/LoopRunnable.h"
#include "../util/ThreadCreator.h"
#include "../util/RequestParameter.h"

class StatusReport: public LoopRunnable {
private:
	int interval;
	RequestParameter parameter;

public:
	StatusReport();
	StatusReport(const RequestParameter& parameter, int inteval = 20000);
	virtual ~StatusReport();

	void SetInterval(int inteval);
	void SetParameter(const RequestParameter& parameter);

	bool Do();
};

#endif /* STATUSREPORT_H_ */
