/*
 * logfile.h
 *
 *  Created on: 2012-11-6
 *      Author: Administrator
 */

#ifndef LOGFILE_H_
#define LOGFILE_H_

#define SCREEN_LOG 0
#define PRINT_LOG 1
#define COMX_LOG 2

void* open(int type);

void write(void* plog, void* data, int len);

void close(void* plog);

#endif /* LOGFILE_H_ */
