/*
 * DrawList.c
 *
 *  Created on: 2012-7-16
 *      Author: Administrator
 */

#include "DrawList.h"
#include <stdio.h>
#include "../util/Util.h"
#include <iostream>
using namespace std;

DrawList::DrawList() :
		Window("") {
}

DrawList::~DrawList() {
}

void DrawList::SetFileNames(const vector<string>& fileNames) {
	this->fileNameList = fileNames;
}

void DrawList::SetBmpFile(string bmpFile) {
	this->bmpFile = bmpFile;
}

void DrawList::OnPaint(HDC hDC) {
	HFONT hFont; //字体句柄
	HBITMAP hBitmap, hOldBitmap;
	HDC hMemDC;
	BITMAP bm;
	int x = 100, y = 180;
	hBitmap = (HBITMAP) LoadImage(NULL, bmpFile.c_str(), IMAGE_BITMAP,
			::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN),
			LR_LOADFROMFILE);
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	hMemDC = CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
	BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	COLORREF crOld = SetTextColor(hDC, RGB(255,0,0));
	hFont = CreateFont(30, 0, //高度20, 宽取0表示由系统选择最佳值
			0, 0, //文本倾斜，与字体倾斜都为0
			FW_HEAVY, //粗体
			0, 0, 0, //非斜体，无下划线，无中划线
			GB2312_CHARSET, //字符集
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, //一系列的默认值
			DEFAULT_PITCH | FF_DONTCARE, "宋体" //字体名称
			);

	SelectObject(hDC, hFont);
	SetBkMode(hDC, TRANSPARENT);
	char numb[4];
	if (this->fileNameList.size() != 0) {
		for (unsigned int i = 0; i < this->fileNameList.size(); i++) {
			if (this->fileNameList.at(i).length() > 0) {
				vector<string> temp = Util::Split(this->fileNameList.at(i),
						'.');
				if (temp.at(0) != "") {
					sprintf(numb, "%d", i + 1);
					string aaa = numb;
					aaa += "." + temp.at(0);
					TextOut(hDC, x, y, aaa.c_str(), strlen(aaa.c_str())); //字体输出
					y = y + 50;
				}
			}
		}
	}
	SetTextColor(hDC, crOld);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);
}

void DrawList::ShowList(const vector<string>& names, int time) {
	string bmpfile = "C:\\program files\\bank\\namelist.bmp";
	DrawList list;
	list.Create();
	Sleep(400);
	list.Show();
	list.SetTopMost(true);
	list.SetVisible(true);
	list.SetFileNames(names);
	list.SetBmpFile(bmpfile);
	list.Refresh();
	Sleep(10000);
	list.SetVisible(false);
}
