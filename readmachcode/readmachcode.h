/*
 * registermach.h
 *
 *  Created on: 2012-12-22
 *      Author: xia
 */

#ifndef REGISTERMACH_H_
#define REGISTERMACH_H_
#include <windows.h>
#include "comapi.h"
#include <string>
using namespace std;
class registermach {
private:

public:
	registermach();
	virtual ~registermach();
	static void GetMachCode(HWND hwnd, int ID, char *machcodes);
	static void *OpenCom(COMPARAM* parm);
	static int GetHash(unsigned char *buf);
	static string GenerateMach(char * localRegistCod, int type);
};

#endif /* REGISTERMACH_H_ */
