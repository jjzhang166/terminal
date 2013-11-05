/*
 * comapi.h
 *
 *  Created on: 2012-9-20
 *      Author: zhangbo
 */

#ifndef COMAPI_H_
#define COMAPI_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _COMPARAM {
	unsigned char bComId;
	unsigned char bByteSize;
	unsigned char bStopBits;
	unsigned char bParity;
	unsigned long dwBaudRate;
	unsigned long fBinary;
	char bEvtChar;
} COMPARAM, *LPCOMPARAM;

void* com_open(LPCOMPARAM param);

unsigned long com_read(void* com, unsigned char* buffer, unsigned long bufLen);

unsigned long com_write(void* com, const unsigned char* data,
		unsigned long len);

int com_close(void* com);

#ifdef  __cplusplus
}
#endif

#endif /* COMAPI_H_ */
