/*
 * modbus.cpp 功能：MODBUS协议层COMMAND构建
 *
 *  Created on: 2013-5-7
 *      Author: zhongshaowei
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * MODBUS指令CRC16校验码
 */
unsigned int crc_cal16 (unsigned char *snd, unsigned char num)
{
   unsigned char i, j;
   unsigned int c,crc=0xFFFF;
   for(i = 0; i < num; i ++)
   {
       c = snd[i] & 0x00FF;
       crc ^= c;
       for(j = 0;j < 8; j ++)
       {
           if (crc & 0x0001)
           {
               crc>>=1;
               crc^=0xA001;
           }
           else crc>>=1;
       }
   }
   return(crc);
}

/*
 * 读取温湿度的MODBUS指令
 */
void getReadHumitureCommand(unsigned char devId, unsigned char *sendBuf)
{
	int crcValue = 0;
	unsigned char tmpData[6];

	memset((char*)sendBuf, 0, 8);
	sendBuf[0] = devId;
	sendBuf[1] = 0x03;
	sendBuf[2] = 0x00;
	sendBuf[3] = 0x00;
	sendBuf[4] = 0x00;
	sendBuf[5] = 0x02;

	memset((char*)tmpData, 0, 6);
	memcpy((char*)tmpData, (char*)sendBuf, 6);
	crcValue = crc_cal16 (tmpData, 6);
	sendBuf[6] = crcValue & 0xff;
	sendBuf[7] = crcValue >> 8;

	/*
    int idx = 0;
	printf("ReadHumitureCommand:");
	while(idx < 8){
		printf("%2x ", sendBuf[idx]);
		idx++;
	}
	printf("\n");
	*/
}

/*
 * 设置从机地址的MODBUS指令
 */
void getSettingAddrCommand(unsigned char srcId, unsigned char dstId, unsigned char *sendBuf)
{
	int crcValue = 0;
	unsigned char tmpData[6];

	memset((char*)sendBuf, 0, 8);
	sendBuf[0] = srcId;
	sendBuf[1] = 0x06;
	sendBuf[2] = 0x00;
	sendBuf[3] = 0x40;
	sendBuf[4] = 0x00;
	sendBuf[5] = dstId;

	memset((char*)tmpData, 0, 6);
	memcpy((char*)tmpData, (char*)sendBuf, 6);
	crcValue = crc_cal16 (tmpData, 6);
	sendBuf[6] = crcValue & 0xff;
	sendBuf[7] = crcValue >> 8;

	/*
    int idx = 0;
	printf("SettingAddrCommand:");
	while(idx < 8){
		printf("%2x ", sendBuf[idx]);
		idx++;
	}
	printf("\n");
	*/
}

