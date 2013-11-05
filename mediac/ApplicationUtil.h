/*
 * ApplicationUtil.h
 *
 *  Created on: 2012-8-26
 *      Author: zhangbo
 */

#ifndef APPLICATIONUTIL_H_
#define APPLICATIONUTIL_H_

#include <windows.h>
#include "../util/Properties.h"
#include <string>
#include "../util/HttpRequest.h"
using namespace std;

class ApplicationUtil {
public:
	ApplicationUtil();
	virtual ~ApplicationUtil();

	static string GetUUID();

	static string SystemUUID();

	static string VersionUUID();

	static bool SendMessages(string message);

	static bool PostMessages(Properties& data);

	static void PlayPhoto(string filePath, unsigned int age);

	static void MouseEvent();

	static string GetValue(string files, char chr, int n);

	static bool CreateRootDir();

	static HttpRequest& InterfaceRequest();

	static HttpRequest& InterfaceRequest(const string& method);

	//static void ShowList(string filenames);

	static RequestParameter InterfaceParameter();

};

#endif /* APPLICATIONUTIL_H_ */
