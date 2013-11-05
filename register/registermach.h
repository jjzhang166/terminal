/*
 * registermach.h
 *
 *  Created on: 2012-12-22
 *      Author: xia
 */

#ifndef REGISTERMACH_H_
#define REGISTERMACH_H_
#include <windows.h>
class registermach {
private:

public:
	registermach();
	virtual ~registermach();
	static void GetMachCode(HWND hwnd, int ID, char *machcodes);
};

#endif /* REGISTERMACH_H_ */
