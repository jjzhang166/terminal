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

class ClientThread: public Runnable {
private:
	SOCKET socket;

public:
	ClientThread(SOCKET socket);
	virtual ~ClientThread();

	int Read(unsigned int socket, char* buffer, int len);
	void SetSocket(SOCKET socket);

	void Run();
};

#endif /* CLIENTTHREAD_H_ */
