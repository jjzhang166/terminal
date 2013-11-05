/*
 * StatusReport.cpp
 *
 *  Created on: 2012-6-28
 *      Author: zhangbo
 */

#include "StatusReport.h"
#include "../util/Util.h"

StatusReport::StatusReport() {
	// TODO Auto-generated constructor stub

}

StatusReport::StatusReport(const RequestParameter& parameter, int inteval) {
	this->parameter = parameter;
	this->interval = inteval;
}

StatusReport::~StatusReport() {
	// TODO Auto-generated destructor stub
}

void StatusReport::SetInterval(int interval) {
	this->interval = interval;
}

void StatusReport::SetParameter(const RequestParameter& parameter) {
	this->parameter = parameter;
}

bool StatusReport::Do() {
	HttpRequest::Create(parameter).GetRelease(10);
	Sleep(this->interval);
	return true;
}

