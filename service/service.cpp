/*
 * service.cpp
 *
 *  Created on: 2013-1-22
 *      Author: xia
 */

//#include "service.h"
//#include <windows.h>
//#include <iostream>
//#include<stdio.h>
//#include<Winsock2.h>
//#include "../terminal/windows/message.h"
////#include "print.h"
//
//using namespace std;
//
//static void* lpt = NULL;
//
//service::service() {
//	// TODO Auto-generated constructor stub
//	running = false;
//}
//
//service::~service() {
//	// TODO Auto-generated destructor stub
//}
//
//void printer_print_buff_init() {
//	//InitializeCriticalSection(&critsec);
//	lpt = lptopen(0x378);
//	if (lpt == (void*) 0) {
//		MessageBox(NULL, ERROR_INITPRINT, TITLE_POINT, MB_ICONERROR | MB_OK);
//	}
//}
//
//void service::InitSocket() {
//	printer_print_buff_init();
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	int err;
//
//	wVersionRequested = MAKEWORD( 2, 2 );
//
//	err = WSAStartup(wVersionRequested, &wsaData);
//	if (err != 0) {
//		return;
//	}
//
//	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2) {
//		WSACleanup();
//		return;
//	}
//	sockserver = socket(AF_INET, SOCK_DGRAM, 0);
//	addrserver.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	addrserver.sin_family = AF_INET;
//	addrserver.sin_port = htons(6000);
//	bind(sockserver, (SOCKADDR*) &addrserver, sizeof(SOCKADDR));
//	len = sizeof(SOCKADDR);
//}
//
//void service::CloseSocket() {
//	lptclose(lpt);
//	closesocket(sockserver);
//	WSACleanup();
//}
//
//void bufchain_init(bufchain *ch) {
//	ch->head = ch->tail = NULL;
//	ch->buffersize = 0;
//}
//
//
//void service::Run() {
//	running = true;
//	InitSocket();
//	while (running) {
//
//		recvfrom(sockserver, recvBuf, 100, 0, (SOCKADDR*) &addrclient, &len);
//		lptwrite(lpt, recvBuf,strlen(recvBuf));
//
//		cout << tempBuf << endl;
//		cout << "input data:" << endl;
//
//	}
//	CloseSocket();
//}
//int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int show) {
//	service* pservice = new service();
//
//	void* handle = ThreadCreator::StartThread(pservice);
//	WaitForSingleObject(handle, INFINITE);
//	return 0;
//
//}
