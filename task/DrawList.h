/*
 * DrawList.h
 *
 *  Created on: 2012-7-16
 *      Author: Administrator
 */

#ifndef DRAWLIST_H_
#define DRAWLIST_H_

#include "../util/Window.h"
#include <string>
#include <vector>
using namespace std;

class DrawList: public Window {
private:
	string desFile;
	string bmpFile;
	vector<string> fileNameList;

public:
	DrawList();

	virtual ~DrawList();

	void OnPaint(HDC hDC);

	void SetBmpFile(string bmpFile);

	void SetFileNames(const vector<string>& names);

	static void ShowList(const vector<string>& names, int time);
};

#endif /* DRAWLIST_H_ */
