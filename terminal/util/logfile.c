/*
 * logfile.c
 *
 *  Created on: 2012-11-6
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include "logfile.h"

static int id[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const static char* names[] = { "./screen%03d.txt", "./print%03d.txt",
		"comy%03d.txt", "comz%03d.txt", "comx%03d.txt" };

void* open(int type) {
	char name[1024];
	sprintf(name, names[type], id[type]++);
	return fopen(name, "w+");
}

void write(void* plog, void* data, int len) {
	fwrite(data, len, 1, plog);
	fflush(plog);
}

void close(void* plog) {
	fclose(plog);
}
