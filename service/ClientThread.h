/*
 * ClientThread.h
 *
 *  Created on: 2013-1-23
 *      Author: zhangbo
 */

#ifndef CLIENTTHREAD_H_
#define CLIENTTHREAD_H_

#include <windows.h>
#include <winsock2.h>
#include "Runnable.h"

#define MAX_COM_COUNT 8

struct client {
	unsigned char lock; // 0: unlock 1: lock
	CRITICAL_SECTION critsec;
	unsigned char ip[4];
	unsigned char screen;
};

class ClientThread: public Runnable {
private:
	static void* printer;
	static struct client coms[MAX_COM_COUNT];
	SOCKET socket;

public:
	ClientThread(SOCKET socket);
	virtual ~ClientThread();

	int Read(unsigned int socket, char* buffer, int len);
	void SetSocket(SOCKET socket);

	void Run();

	static void Initialize();
	static void Uninitialize();
};

#endif /* CLIENTTHREAD_H_ */
