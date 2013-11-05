/*
 * printapi.h
 *
 *  Created on: 2013-9-11
 *      Author: zhangbo
 */

#ifndef PRINTAPI_H_
#define PRINTAPI_H_

#define PRINT_DRV 0
#define PRINT_COM 1
#define PRINT_SHR 2
#define PRINT_FIL 3
#define PRINT_LPT 4

void* print_new(char *name, char type, char whole);

void print_open(void* printer);
void print_data(void* printer, void* data, int len);
void print_close(void* printer);

void print_del(void* printer);

#endif /* PRINTAPI_H_ */
