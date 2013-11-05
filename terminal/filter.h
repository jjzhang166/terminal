/*
 * Extends.h
 *
 *  Created on: 2012-9-26
 *      Author: zhangbo
 */

#ifndef EXTENDS_H_
#define EXTENDS_H_
#ifdef __cplusplus
extern "C" {
#endif
int com_setup(char com, int baudRate, int parity, int byteSize, int stopBits);

void com_read_proc_start();
void com_read_proc_stop();

int com_get_output();
void com_set_enable(int state);
void com_set_output(int state);
void com_set_line_mode(int state);
void com_set_direct_mode(int state);
void com_receive(unsigned char* data, unsigned long len);

#ifdef __cplusplus
}
#endif

#endif /* EXTENDS_H_ */
