/*
 * remote.cpp 功能：远程客户端通信控制管理与数据采集
 *
 *  Created on: 2013-5-6
 *      Author: zhongshaowei
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <Winsock2.h>
#include "remote.h"
#include "modbus.h"
#include "collection.h"
#include "tcp.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;

unsigned int remote_open(const char* ip, int port) {
	WSADATA wsd;
	SOCKET sClient;
	struct sockaddr_in server;
	struct hostent *host = NULL;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0) {
		return 0;
	}
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET) {
		return 0;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip);
	if (server.sin_addr.s_addr == INADDR_NONE) {
		host = gethostbyname(ip);
		if (host == NULL) {
			return 0;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}
	int m = connect(sClient, (struct sockaddr *) &server, sizeof(server));
	if (m == SOCKET_ERROR) {
		return 0;
	}

	return sClient;
}

unsigned int remote_recv(unsigned int handle, void* data, int maxlen) {
	int ret = 0;
	do {
		ret = recv(handle, (char*) data, maxlen, 0);
		if (ret > 0) {
			break;
		}
	} while (false);
	return ret;
}

unsigned int remote_send(unsigned int handle, const void* data, int len) {
	do {
		if (send(handle, (const char*) data, len, 0) == 0) {
			break;
		}
	} while (false);

	return 0;
}

void remote_close(unsigned int sClient) {
	closesocket(sClient);
	WSACleanup();
}

void mainmain() {
	int recvLen = 0;
	sendPackage sendInfo;
	unsigned int handle = 0;
	unsigned char recvData[100];
    unsigned char sendData[TCP_SEND_LEN+1];
    int ret = 0;
    int idx = 0;
	float temValue = 0;
	float RHValue = 0;

    //zigbeeDeviceScanning((char *)IPtable[0].IPaddr, IPtable[0].port);

	handle = remote_open((char *)IPtable[0].IPaddr, IPtable[0].port);

	/********************start 读取从机0x01的温湿度**********************/
	//1.构建读取温湿度的请求报文
	memset(&sendInfo, 0, sizeof(sendInfo));
	getReadHumiturePackage(&masterTable[1], slaveTable[1], &sendInfo);
	memset(sendData, 0, TCP_SEND_LEN+1);
	memcpy(sendData, &sendInfo, TCP_SEND_LEN);

	//2.发送TCP/IP请求报文
	remote_send(handle, sendData, TCP_SEND_LEN);

	//3.接收TCP/IP响应报文
	memset(recvData, 0, 100);
	recvLen = remote_recv(handle, recvData, 100);

	//4.获取实际温湿度值
	temValue = 0;
	RHValue = 0;
	ret = getHumitureValue(&temValue, &RHValue, recvData, recvLen);
	if ( ret ==0 ){
		printf("Slave addr 0x01(Temperation:%.2f, RH:%.2f)\n",temValue, RHValue);
	}
	else{
		printf("Get slave 0x01 humiture value failed.\n");
	}
	/********************end 读取从机0x01的温湿度**********************/


	/********************start 读取从机0x02的温湿度**********************/
	//1.构建读取温湿度的请求报文
	memset(&sendInfo, 0, sizeof(sendInfo));
	getReadHumiturePackage(&masterTable[1], slaveTable[2], &sendInfo);
	memset(sendData, 0, TCP_SEND_LEN+1);
	memcpy(sendData, &sendInfo, TCP_SEND_LEN);

	//2.发送TCP/IP请求报文
	remote_send(handle, sendData, TCP_SEND_LEN);

	//3.接收TCP/IP响应报文
	memset(recvData, 0, 100);
	recvLen = remote_recv(handle, recvData, 100);

	//4.获取实际温湿度值
	temValue = 0;
	RHValue = 0;
	ret = getHumitureValue(&temValue, &RHValue, recvData, recvLen);
	if ( ret ==0 ){
		printf("Slave addr 0x02(Temperation:%.2f, RH:%.2f)\n",temValue, RHValue);
	}
	else{
		printf("Get slave 0x02 humiture value failed.\n");
	}
	/********************end 读取从机0x02的温湿度**********************/

/*
	//1.构建设置从机地址的请求报文
	unsigned char srcId = slaveAddr[1];
	unsigned char dstId = slaveAddr[2];
	memset(&sendInfo, 0, sizeof(sendInfo));
	getSettingAddrPackage(&devTable[1], srcId, dstId, &sendInfo);
	memset(sendData, 0, TCP_SEND_LEN+1);
	memcpy(sendData, &sendInfo, TCP_SEND_LEN);

	//2.发送TCP/IP请求报文
	remote_send(handle, sendData, TCP_SEND_LEN);

	//3.接收TCP/IP响应报文
	memset(recvData, 0, 100);
	recvLen = remote_recv(handle, recvData, 100);

	//4.获取从机地址修改结果
	unsigned char newAddr;
	ret = getSlaveAddrNewValue(&newAddr, recvData, recvLen);
	if ( ret ==0 ){
		printf("Slave new addr:0x%x\n",newAddr);
	}
	else{
		printf("Get Slave new addr failed.\n");
	}
*/
	/*******************************
	printf("ADDR_SendData(%d):", TCP_SEND_LEN);
	idx = 0;
	while(idx < TCP_SEND_LEN){
		printf("%2x ", sendData[idx]);
		idx++;
	}
	printf("\n");
	/*******************************/
	/*******************************
	printf("ADDR_RecvData:" );
	idx = 0;
	while(idx < 24){
		printf("%2x ", recvData[idx]);
		idx++;
	}
	printf("\n");
	/*******************************/
	/*******************************/
	/*
	printf("RecvData(%d):", recvLen);
	idx = 0;
	while(idx < recvLen){
		printf("%2x ", recvData[idx]);
		idx++;
	}
	printf("\n");
	*/
	/*******************************/
	remote_close(handle);
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int show) {

	mainmain();
	return 0;
}
