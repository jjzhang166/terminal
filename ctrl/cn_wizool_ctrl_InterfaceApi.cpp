/*
 * cn_wizool_ctrl_InterfaceApi.c
 *
 *  Created on: 2013-4-27
 *      Author: zhangbo
 */

#include "cn_wizool_ctrl_InterfaceApi.h"
#include "collection.h"
#include "modbus.h"
#include "remote.h"
#include "tcp.h"
#include "string.h"
#include "stdio.h"

JNIEXPORT jint JNICALL Java_cn_wizool_ctrl_InterfaceApi_add(JNIEnv * evn,
		jobject obj, jint a, jint b)
{
	return a + b;
}

/*
 * 打开客服端
 */
JNIEXPORT jint JNICALL Java_cn_wizool_ctrl_InterfaceApi_remoteOpen(JNIEnv * evn,
		jobject obj, _jstring *clientIP, jint port)
{
	unsigned int handle = 0;

	handle = remote_open((char *)clientIP, port);
	if (handle != 0){
		return handle;
	}
	else{
		return 0;
	}
}

/*
 * 关闭客服端
 */
JNIEXPORT void JNICALL Java_cn_wizool_ctrl_InterfaceApi_remoteClose(JNIEnv * evn,
		jobject obj, jint handle)
{
	remote_close(handle);
}


/*
 * 查询设备的从机的温度
 */
JNIEXPORT float JNICALL Java_cn_wizool_ctrl_InterfaceApi_getTemperature(JNIEnv * evn,
		jobject obj, jint masterAddr, jint slaveAddr)
{
	int recvLen = 0;
	sendPackage sendInfo;
	unsigned int handle = 0;
	unsigned char recvData[100];
    unsigned char sendData[TCP_SEND_LEN+1];
    int ret = 0;
	float temValue = 0;
	float RHValue = 0;

	//1.构建读取温湿度的请求报文
	memset(&sendInfo, 0, sizeof(sendInfo));
	getReadHumiturePackage(&masterTable[masterAddr], slaveTable[slaveAddr], &sendInfo);
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
		printf("Slave addr 0x%x(Temperation:%.2f, RH:%.2f)\n",slaveAddr,temValue, RHValue);
		return temValue;
	}
	else{
		printf("Get slave 0x%x humiture value failed.\n", slaveAddr);
		return 0;
	}
}

/*
 * 查询设备的从机的湿度
 */
JNIEXPORT float JNICALL Java_cn_wizool_ctrl_InterfaceApi_getHumidity(JNIEnv * evn,
		jobject obj, jint masterAddr, jint slaveAddr)
{
	int recvLen = 0;
	sendPackage sendInfo;
	unsigned int handle = 0;
	unsigned char recvData[100];
    unsigned char sendData[TCP_SEND_LEN+1];
    int ret = 0;
	float temValue = 0;
	float RHValue = 0;

	//1.构建读取温湿度的请求报文
	memset(&sendInfo, 0, sizeof(sendInfo));
	getReadHumiturePackage(&masterTable[masterAddr], slaveTable[slaveAddr], &sendInfo);
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
		printf("Slave addr 0x%x(Temperation:%.2f, RH:%.2f)\n",slaveAddr,temValue, RHValue);
		return RHValue;
	}
	else{
		printf("Get slave 0x%x humiture value failed.\n", slaveAddr);
		return 0;
	}
}

/*
 * 设置设备的从机地址
 */
JNIEXPORT jint JNICALL Java_cn_wizool_ctrl_InterfaceApi_setSlaveAddr(JNIEnv * evn,
		jobject obj, jint masterAddr, jbyte srcAddr, jbyte dstAddr)
{
	int recvLen = 0;
	sendPackage sendInfo;
	unsigned int handle = 0;
	unsigned char recvData[100];
    unsigned char sendData[TCP_SEND_LEN+1];
    int ret = 0;

	//1.构建设置从机地址的请求报文
	unsigned char srcId = srcAddr;
	unsigned char dstId = dstAddr;
	memset(&sendInfo, 0, sizeof(sendInfo));
	getSettingAddrPackage(&masterTable[masterAddr], srcId, dstId, &sendInfo);
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
		return 0;
	}
	else{
		printf("Get Slave new addr failed.\n");
		return 1;
	}
}
