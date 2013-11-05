/*
 * comapi.h
 *
 *  Created on: 2013-5-6
 *      Author: zhongshaowei
 */

#ifndef COLLECTION_H_
#define COLLECTION_H_

int getHumitureValue(float *temValue, float *RHValue, unsigned char *recvBuf, int cmdLen);
int getSlaveAddrNewValue(unsigned char *newAddr, unsigned char *recvBuf, int cmdLen);

#endif /* COLLECTION_H_ */
