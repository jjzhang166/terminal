/*
 * tcp.cpp
 *
 *  Created on: 2013-5-10
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

unsigned char header[2] = {0x7e, 0x00};
unsigned char sendFlag[2] = {0x10,0x00};
unsigned char recvFlag[2] = {0x90};

//TCP客服端通信端口
clientInfo IPtable[] = {
		{10002,"192.168.1.21"}
};

//ZIGBEE数据采集器信息(1080A)
zigbeeInfo masterTable[10] = {
		{0x00,0x13,0xa2,0x00,0x40,0x65,0x02,0x0a, 0xc1,0xd6,0x00,0x00},
		{0x00,0x13,0xa2,0x00,0x40,0x67,0xb5,0x64, 0x04,0x54,0x00,0x00}
};

//采集模块地址即从机地址表
unsigned char slaveTable[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

/*
 * zigbee数据采集设备扫描
 *
 * （1）启动扫描网内串口设备请求：
 * 7E 00 04 08 52 4E 44 13
 * （2）返回网内串口设备信息响应：
 * 7E 00 1A 88 52 4E 44 00 "04 54" "00 13 A2 00 40 67 B5 64" 64 65 00 FF FE 01 00 C1 05 10 1E 0B   (30)
 * 7E 00 19 88 52 4E 44 00 "C1 D6" "00 13 A2 00 40 65 02 0A" 62 00 FF FE 01 00 C1 05 10 1E 42   (29)
 */
void zigbeeDeviceScanning(char *IPaddr, unsigned int port){
	int recvLen = 0;
	int idx = 0;
	unsigned int handle = 0;
	unsigned char recvData[100];
	unsigned char devScanCmd[] = {0x7e, 0x00, 0x04, 0x08, 0x52, 0x4E, 0x44, 0x13};

	handle = remote_open(IPaddr, port);

	//2.发送COM口扫描报文
	remote_send(handle, devScanCmd, 8);

	//3.接收扫描到的COM口报文
	while(idx < 2){
		memset(recvData, 0, 100);
		recvLen = remote_recv(handle, recvData, 100);
		if (recvLen > 0){
			memcpy(masterTable[idx].sendNI, (char *)recvData+8, 2);
			memcpy(masterTable[idx].ID64Bit, (char *)recvData+10, 8);
			idx++;
			printf("find the %d com.\n", idx);
		}
	}

	unsigned char tmp[13];
	memset(tmp, 0, 13);
	memcpy(tmp, (void *)&masterTable[0], 12);
	printf("return 1 com info:");
	idx = 0;
	while(idx < 12){
		printf("%2x ", tmp[idx]);
		idx++;
	}
	printf("\n");
	memset(tmp, 0, 13);
	memcpy(tmp, (void *)&masterTable[1], 12);
	printf("return 2 com info:");
	idx = 0;
	while(idx < 12){
		printf("%2x ", tmp[idx]);
		idx++;
	}
	printf("\n");

	remote_close(handle);
}

/*
 * 读温湿度的TCP报文
 */
void getReadHumiturePackage(zigbeeInfo *devInfo, unsigned char slaveAddr, sendPackage *sendData){

	int idx = 0;
	unsigned char sum = 0;
	unsigned char tmpData[TCP_SEND_LEN+1];


	memset(sendData, 0, TCP_SEND_LEN+1);
	memcpy((void*)(sendData->header), (void*)header, PACKAGE_HEADER_LEN);
	sendData->length = SEND_PACKAGE_LEN;
	memcpy((void*)(sendData->sendFlags), (void*)sendFlag, SEND_FLAG_LEN);
	memcpy((void*)(sendData->ID64Bit), (void*)devInfo->ID64Bit, SEND_DEVID_LEN);
	memcpy((void*)(sendData->NIName), (void*)devInfo->sendNI, SEND_NI_LEN);
	getReadHumitureCommand(slaveAddr, sendData->modbusCmd);

	memset(tmpData, 0, TCP_SEND_LEN);
	memcpy(tmpData, sendData, TCP_SEND_LEN);
	while(idx < SEND_PACKAGE_LEN){
		sum = sum+tmpData[idx+3];
		idx++;
	}
	sendData->verify = 0xff - sum;
	memset(tmpData, 0, TCP_SEND_LEN+1);
	memcpy(tmpData, sendData, TCP_SEND_LEN);
}

/*
 * 设置从机地址的TCP报文
 */
void getSettingAddrPackage(zigbeeInfo *devInfo, unsigned char srcId, unsigned char dstId, sendPackage *sendData){

	int idx = 0;
	unsigned char sum = 0;
	unsigned char tmpData[TCP_SEND_LEN+1];


	memset(sendData, 0, TCP_SEND_LEN+1);
	memcpy((void*)(sendData->header), (void*)header, PACKAGE_HEADER_LEN);
	sendData->length = SEND_PACKAGE_LEN;
	memcpy((void*)(sendData->sendFlags), (void*)sendFlag, SEND_FLAG_LEN);
	memcpy((void*)(sendData->ID64Bit), (void*)devInfo->ID64Bit, SEND_DEVID_LEN);
	memcpy((void*)(sendData->NIName), (void*)devInfo->sendNI, SEND_NI_LEN);
	getSettingAddrCommand(srcId, dstId, sendData->modbusCmd);

	memset(tmpData, 0, TCP_SEND_LEN);
	memcpy(tmpData, sendData, TCP_SEND_LEN);
	while(idx < SEND_PACKAGE_LEN){
		sum = sum+tmpData[idx+3];
		idx++;
	}
	sendData->verify = 0xff - sum;
	memset(tmpData, 0, TCP_SEND_LEN+1);
	memcpy(tmpData, sendData, TCP_SEND_LEN);
}


