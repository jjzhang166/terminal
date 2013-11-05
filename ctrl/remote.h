/*
 * remote.h
 *
 *  Created on: 2013-5-6
 *      Author: zhongshaowei
 */

#ifndef REMOTE_H_
#define REMOTE_H_

unsigned int remote_open(const char* ip, int port);
void remote_close(unsigned int handle);
unsigned int remote_recv(unsigned int handle, void* data, int maxlen);
unsigned int remote_send(unsigned int handle, const void* data, int len);

#endif /* REMOTE_H_ */
