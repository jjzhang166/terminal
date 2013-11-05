/*
 * Scrolling.cpp
 *
 *  Created on: 2012-7-27
 *      Author: Administrator
 */

#include "Scrolling.h"
#include "../util/Util.h"
#include "MediaApplication.h"

#include <string>
using namespace std;

Scrolling::Scrolling(void) {
	running = false;
}

Scrolling::~Scrolling(void) {

}

BOOL Scrolling::StartScrolling(HDC hdc, HDC m_dcMemoryDC, char* szText, int len,
		LPSIZE s, int width1, int heigth1, LPRECT rect,
		HBRUSH m_hBrushBackground) {
	static int width = width1, heigth = heigth1, kuandu = width1 + s->cx, i = 0;
	FillRect(m_dcMemoryDC, rect, m_hBrushBackground);
	SetBkMode(m_dcMemoryDC, TRANSPARENT);
	TextOut(m_dcMemoryDC, width - i, heigth - 35, szText, len);
	BitBlt(hdc, 0, heigth - 40, width, 40, m_dcMemoryDC, 0, heigth - 40,
			SRCCOPY);
	i++;
	if (i == kuandu) {
		width = GetSystemMetrics(0);
		i = 0;
	}

	Sleep(10); //睡眠10毫秒
	return TRUE;
}

void Scrolling::Run() {
	running = true; //ShowScrolling();
	int width = GetSystemMetrics(0);
	int heigth = GetSystemMetrics(1);
	SIZE s;
	RECT rect;
	rect.bottom = heigth;
	rect.left = 0;
	rect.right = width;
	rect.top = heigth - 40;
	HDC hdc = GetDC(NULL);
	HDC m_dcMemoryDC = CreateCompatibleDC(hdc); //创建内存DC
	HBITMAP m_hMemoryBitmap = CreateCompatibleBitmap(hdc, width, heigth); //创建内存Bitmap
	SetTextColor(m_dcMemoryDC, RGB(255,0,0));
	SetBkColor(m_dcMemoryDC, RGB(193,255,209));
	SelectObject(m_dcMemoryDC, m_hMemoryBitmap);
	HBRUSH m_hBrushBackground = CreateSolidBrush(RGB(193,255,209));
	HFONT m_hFont = CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH, "黑体");
	SelectObject(m_dcMemoryDC, m_hFont);
	int i;
	while (running) {
		string msg = MediaApplication::scrollInfo->Get("message");
		if ((msg != "")) {
			msg += "\n";
			char szText[1024];
			int len1 = lstrlen(msg.c_str());

			for (i = 0; i < len1 + 1; i++) {
				szText[i] = msg.c_str()[i];
			}
			int len = lstrlen(szText);
			GetTextExtentPoint(m_dcMemoryDC, szText, len, &s); //计算字符串占的像素
			this->StartScrolling(hdc, m_dcMemoryDC, szText, len, &s, width,
					heigth, &rect, m_hBrushBackground);
		}
	}
}

void Scrolling::Stop() {
	running = false;
}

