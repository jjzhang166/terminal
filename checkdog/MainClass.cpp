/*
 * MainClass.cpp
 *
 *  Created on: Jun 16, 2012
 *      Author: zhangbo
 */

#include "../util/Util.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
	string reboot = "C:\\Program Files\\bank\\reboot.txt";
	string old = Util::ReadFile(reboot, 36);
	while (true) {
		if (Util::Download("64.112.8.10", "/bank/reboot.txt", 80, reboot)) {
			string now = Util::ReadFile(reboot, 36);
			if (old != now) {
				Util::Reboot();
			}
		}
		Sleep(10 * 60 * 1000);
	}

	return (0);
}
