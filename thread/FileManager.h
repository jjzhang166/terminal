/*
 * FileManager.h
 *
 *  Created on: 2012-8-28
 *      Author: zhangbo
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <vector>
#include <string>
#include "../util/RequestParameter.h"
#include <windows.h>
using namespace std;

class FileManager {
private:
	CRITICAL_SECTION critsec;
	string temp;
	string path;
	RequestParameter parameter;

public:
	FileManager();
	virtual ~FileManager();

	void SetTempDirectory(const string& temp);
	void SetDirectory(const string& path);
	void SetRequestParameter(const RequestParameter& param);

	void RemoveFile(const string& fid);
	string DownFile(const string& fid);
	bool ExistsFile(const string& fid);
	void DownFile(vector<string>& fids);

};

#endif /* FILEMANAGER_H_ */
