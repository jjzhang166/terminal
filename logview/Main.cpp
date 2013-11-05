/*
 * Main.cpp
 *
 *  Created on: 2013-4-25
 *      Author: zhangbo
 */

#include "../winutil/des.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

unsigned char key[] = "!@#$%^&*";

int main(int argc, char *argv[]) {
	unsigned char buffer[1024*16];
	if (argc != 3) {
		cout << "logview <enc.log> <dec.log>" << endl;
		exit(0);
	}
	FILE* fr = fopen(argv[1], "rb");
	FILE* fw = fopen(argv[2], "wb");
	size_t l = 0;
	while (true) {
		l = fread(buffer, 1, 40, fr);
		if (l != 40) {
			break;
		}
		fwrite(buffer, 40, 1, fw);
		buffer[40] = 0;
		char* size = (char*) (buffer + 33);
		int s = 0;
		int ss = 0;
		sscanf(size, "%05d\r\n", &s);
		ss = s;
		if (s % 8 > 0) {
			ss = (s / 8 + 1) * 8;
		}
		l = fread(buffer, 1, ss + 4, fr);

		int i = 0;
		unsigned char buf[8];
		fwrite("(", 1, 1, fw);
		for (i = 0; i < ss / 8; i++) {
			des(buffer + (i * 8), key, buf, DES_DECRYPT);
			if ((i == (ss / 8 - 1)) && (s % 8) > 0) {
				fwrite(buf, (s % 8), 1, fw);
			} else {
				fwrite(buf, 8, 1, fw);
			}
		}
		fwrite(")\r\n\r\n", 5, 1, fw);
		fflush(fw);
	}
	fclose(fr);
	fclose(fw);
}
