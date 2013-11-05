/*
 * ClientThread.cpp
 *
 *  Created on: 2013-1-23
 *      Author: zhangbo
 */

#include "ClientThread.h"
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
using namespace std;

ClientThread::ClientThread(SOCKET socket) {
	this->socket = socket;
}

ClientThread::~ClientThread() {
}

void ClientThread::SetSocket(SOCKET socket) {
	this->socket = socket;
}

int ClientThread::Read(unsigned int socket, char* buffer, int len) {
	int rvCount = 0;
	do {
		rvCount = recv(socket, buffer, len, 0);
		if (rvCount <= 0) {
			return 0;
		}
	} while (false);
	return rvCount;
}

#define BUFFER_SIZE 1024*1024*5
char buffer[BUFFER_SIZE];

void ClientThread::Run() {
	char buf000[128];
	char buf001[] = { 0xFF, 0xFD, 0x01, 0xFF, 0xFD, 0x1F, 0xFF, 0xFB, 0x01,
			0xFF, 0xFB, 0x03 };
	char buf002[] = { 0xFF, 0xFD, 0x03 };
	send(socket, buf001, 12, 0);

	while (1) {
		int rvCount = recv(socket, buffer, BUFFER_SIZE, 0);
		if (rvCount <= 0) {
			return;
		}
		if (rvCount >= 3 && 0 == memcmp(buffer + rvCount - 3, buf002, 3)) {
			FILE* file = fopen("data.txt", "rb");
			size_t l = fread(buffer, 1, BUFFER_SIZE, file);
			fclose(file);
			send(socket, buffer, l, 0);
		}
	}
	return;
}
