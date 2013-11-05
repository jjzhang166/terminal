/*
 * tcp.h
 *
 *  Created on: 2013-5-10
 *      Author: zhongshaowei
 */

#ifndef TCP_H_
#define TCP_H_


#define PACKAGE_HEADER_LEN		2

#define SEND_PACKAGE_LEN	0x16

#define SEND_FLAG_LEN		2

#define RECV_FLAG_LEN		2

#define SEND_DEVID_LEN	8
#define SEND_NI_LEN		4

#define SEND_CMD_LEN		8

#define RECV_NI_LENGTH 		3

#define RECV_FLAG_LENGTH	1
#define TCP_SEND_LEN		26

//TCP客服端通讯端口（IP和PORT）
typedef struct{
	unsigned int port;
	unsigned char IPaddr[16];
}clientInfo;

//ZIGBEE数据采集器信息（1080A）
typedef struct{
	unsigned char ID64Bit[8];
	unsigned char sendNI[4];
}zigbeeInfo;

//TCP请求报文
typedef struct{
	unsigned char header[2];
	unsigned char length;
	unsigned char sendFlags[2];
	unsigned char ID64Bit[8];
	unsigned char NIName[4];
	unsigned char modbusCmd[8];
	unsigned char verify;
}sendPackage;

//TCP响应报文（温湿度）
typedef struct{
	unsigned char header[2];
	unsigned char length;
	unsigned char recvFlags;
	unsigned char ID64Bit[8];
	unsigned char NIName[3];
	unsigned char modbusCmd[9];
	unsigned char verify;
}recvTRPackage;

extern clientInfo IPtable[];
extern zigbeeInfo masterTable[10];
extern unsigned char slaveTable[];

void zigbeeDeviceScanning(char *IPaddr, unsigned int port);
void getReadHumiturePackage(zigbeeInfo *devInfo, unsigned char slaveAddr, sendPackage *sendData);
void getSettingAddrPackage(zigbeeInfo *devInfo, unsigned char srcId, unsigned char dstId, sendPackage *sendData);

#endif /* TCP_H_ */
