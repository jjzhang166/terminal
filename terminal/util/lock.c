/*
 * lock.cpp
 *
 *  Created on: 2012-9-24
 *      Author: zhangbo
 */

#include "lock.h"

#include <windows.h>

static CRITICAL_SECTION critsec;

void open_lock() {
	InitializeCriticalSection(&critsec);
}

void close_lock() {
	DeleteCriticalSection(&critsec);
}

void lock() {
	EnterCriticalSection(&critsec);
}

void unlock() {
	LeaveCriticalSection(&critsec);
}
