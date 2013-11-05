/*
 * comapi.h
 *
 *  Created on: 2013-5-6
 *      Author: zhongshaowei
 */

#ifndef MODBUS_H_
#define MODBUS_H_

unsigned int crc_cal16 (unsigned char *snd, unsigned char num);
void getReadHumitureCommand(unsigned char  devId, unsigned char *sendBuf);
void getSettingAddrCommand(unsigned char srcId, unsigned char dstId, unsigned char *sendBuf);

#endif /* MODBUS_H_ */
