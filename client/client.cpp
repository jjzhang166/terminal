#include<iostream>
#include<string>
#include <stdio.h>
#include<Winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#include <stdlib.h>

#define COM_READ_BUFFER_SIZE 1024
static char tempBuffer[COM_READ_BUFFER_SIZE * 10];
static long tempLen = 0;

void com_send(const char* buf, long len) {
	for (long i = 0; i < len; i++) {
		printf("0x%02X ", buf[i]);
	}
	printf("\r\n");
}

void func(const char* inBuffer, long inLen) {
	int last = 0, i = 0;

	for (i = 0; i < inLen; i++) {
		if (inBuffer[i] == 0x0D) {
			memcpy(tempBuffer + tempLen, inBuffer + last, i + 1 - last);
			tempLen += i + 1 - last;
			last = i + 1;
			com_send(tempBuffer, tempLen);
			tempLen = 0;
		}
	}
	memcpy(tempBuffer + tempLen, inBuffer + last, i - last);
	tempLen += i - last;
}


int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int show) {
	const char inBuffer1[] = "1";
	const char inBuffer2[] = "\r12";
	const char inBuffer3[] = "34\r";
	const char inBuffer4[] = "4";
	func(inBuffer1, strlen(inBuffer1));
	func(inBuffer2, strlen(inBuffer2));
	func(inBuffer3, strlen(inBuffer3));
	func(inBuffer4, strlen(inBuffer4));
	return 0;
}
