/*
 * MainClasss.cpp
 *
 *  Created on: 2013-3-15
 *      Author: xia
 */
#include "../util/Util.h"
#include <stdio.h>
#include "../util/Properties.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
//	for (int i = 0; i < 10000; i++) {
//		Properties prop;
//		prop.SafeLoad("config.txt");
//		int a = prop.GetInteger("a", 0);
//		for (int j = 0; j < 200; j++) {
//			char s[256];
//			sprintf(s, "a%d", j);
//			if (a != prop.GetInteger(s, 0)) {
//				exit(0);
//			}
//			prop.PutInteger(s, a + i);
//		}
//		prop.PutInteger("a", a + i);
//
//		prop.SafeSave("config.txt");
//	}

	char currentpath[128];
	Util::GetCurrentPath(currentpath);

	char exepath[128];
	string user;
	string pass;
	char section[128];
	if (strlen(cmdline) == 0) {
		strcpy(section, "default");
	} else {
		strcpy(section, cmdline);
	}

	char configpath[128];
	sprintf(configpath, "%s\\config.txt", currentpath);
	Properties properties;
	properties.SafeLoad(configpath);
	user = properties.GetString("SysUser", "");
	pass = properties.GetString("SysPass", "");
	int currentuser = properties.GetInteger("CurrentUser", 1);
	string exename = properties.GetString("Exename", "terminal.exe");

	if ((strlen(user.c_str()) == 0) || (strlen(pass.c_str()) == 0)
			|| (currentuser == 1)) {
		sprintf(exepath, "%s\\%s", currentpath, exename.c_str());
		Util::ShellExecuteNoWait(exepath, section);
	} else {
		sprintf(exepath, "%s\\%s %s", currentpath, exename.c_str(), section);
		Util::ShellExecuteLogon(user.c_str(), pass.c_str(), exepath, ".");
	}

	return 0;

}
