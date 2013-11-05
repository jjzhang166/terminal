/*
 * service.cpp
 *
 *  Created on: 2013-1-22
 *      Author: xia
 */

#include "ServiceThread.h"

#include "ThreadCreator.h"
#include "ClientThread.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <winsock2.h>

using namespace std;

ServiceThread::ServiceThread() {
}

ServiceThread::~ServiceThread() {
}

void ServiceThread::Run() {
	SOCKET sockfd, new_fd = 0;
	unsigned short port = 23;
	char yes = 1;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size = 0;

	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest = MAKEWORD(1,1);
	WSAStartup(myVersionRequest, &wsaData);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		return;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		return;
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
			== -1) {
		return;
	}

	if (listen(sockfd, 100) == -1) {
		return;
	}

	while (1) {
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size))
				== -1) {
			continue;
		}
		ThreadCreator::StartThread(new ClientThread(new_fd));
	}

	WSACleanup();
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int show) {
	ServiceThread* server = new ServiceThread();
	void* handle = ThreadCreator::StartThread(server);
	WaitForSingleObject(handle, INFINITE);
	return 0;
}
