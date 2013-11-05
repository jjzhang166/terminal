/*
 * collection.cpp 功能:采集模块的数据转换输出
 *
 *  Created on: 2013-5-7
 *      Author: zhongshaowei
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modbus.h"

/******获取温、湿度实际值********/
int getHumitureValue(float *temValue, float *RHValue, unsigned char *recvBuf, int cmdLen)
{
	int crcValue = 0;
	int crcLo = 0;
	int crcHi = 0;
	float T_Hi;
	float T_Lo;
	float RH_Hi;
	float RH_Lo;
	unsigned char tmpData[9];

	memcpy((char*)tmpData, (char*)recvBuf+15, 9);
	crcValue = crc_cal16 (tmpData, 7);
	crcLo = crcValue & 0xff;
	crcHi = crcValue >> 8;
	//printf("\ncrcValue:%x, crcLo:%x, crcHi:%x\n",crcValue, crcLo, crcHi);
	if ((crcLo == tmpData[7]) && (crcHi == tmpData[8])){
		T_Hi = tmpData[3];
		T_Lo = tmpData[4];
		RH_Hi = tmpData[5];
		RH_Lo = tmpData[6];

		*temValue = (T_Hi*256 + T_Lo) / 10;
		*RHValue = ( RH_Hi*256 + RH_Lo) / 10;

		return 0;
	}
	else{
		return 1;
	}
}

/******获取从机地址修改后结果********/
int getSlaveAddrNewValue(unsigned char *newAddr, unsigned char *recvBuf, int cmdLen)
{
	int crcValue = 0;
	int crcLo = 0;
	int crcHi = 0;
	unsigned char tmpData[8];

	memcpy((char*)tmpData, (char*)recvBuf+15, 8);
	crcValue = crc_cal16 (tmpData, 6);
	crcLo = crcValue & 0xff;
	crcHi = crcValue >> 8;
	//printf("\ncrcValue:%x, crcLo:%x, crcHi:%x, addr:%x, tmpData[6]:%x, tmpData[7]:%x\n",crcValue, crcLo, crcHi, tmpData[5],tmpData[6],tmpData[7]);
	if ((crcLo == tmpData[6]) && (crcHi == tmpData[7])){
		*newAddr = tmpData[5];

		return 0;
	}
	else{
		return 1;
	}
}
