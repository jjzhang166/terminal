/*
 * com.cpp
 *
 *  Created on: 2012-9-20
 *      Author: zhangbo
 */

#include "com.h"

#include <windows.h>

static HANDLE com = NULL;
static COMPARAM comParam;

int current_com_setup(LPCOMPARAM comPtr) {
	memcpy(&comParam, comPtr, sizeof(COMPARAM));
	return 0;
}

int current_com_open() {
	if (com != NULL) {
		com_close(com);
		com = NULL;
	}
	com = com_open(&comParam);
	if (com == NULL) {
		return -1;
	}
	return 0;
}

int current_com_close() {
	if (com != NULL) {
		com_close(com);
		com = NULL;
		return 0;
	}
	return -1;
}

int current_com_write(unsigned char *writeStr, long strLen) {
	if (com != NULL) {
		return com_write(com, writeStr, strLen);
	} else {
		return -1;
	}
}

unsigned long current_com_read(unsigned char *readStr, unsigned long maxStrLen) {
	unsigned long ret = 0;
	if (com != NULL) {
		ret = com_read(com, readStr, maxStrLen);
	}
	return ret;
}
