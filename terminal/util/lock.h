/*
 * lock.h
 *
 *  Created on: 2012-9-24
 *      Author: zhangbo
 */

#ifndef LOCK_H_
#define LOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

void open_lock();

void lock();

void unlock();

void close_lock();

#ifdef __cplusplus
}
#endif

#endif /* LOCK_H_ */
