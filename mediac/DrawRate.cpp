/*
 * DrawRate.cpp
 *
 *  Created on: 2012-7-16
 *      Author: Administrator
 */

#include "DrawRate.h"
#include "Config.h"
#include <stdio.h>
#include "../util/Util.h"
#include "MediaApplication.h"
#include "ApplicationUtil.h"

DrawRate::DrawRate(string title) :
		Window(title) {

	desfile = Config::InstallPath + "tempdes.ini";
}

DrawRate::~DrawRate() {
}

void DrawRate::SetBmpFile(string bmpFile) {
	this->bmpFile = bmpFile; // Config::InstallPath + bmpFile;
}

void DrawRate::SetSection(string section) {
	this->section = section;
}

void DrawRate::OnPaint(HDC hDC) {
	HFONT hFont;
	HBITMAP hBitmap, hOldBitmap;
	HDC hMemDC;
	BITMAP bm;
	hBitmap = (HBITMAP) LoadImage(NULL, bmpFile.c_str(), IMAGE_BITMAP,
			::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN),
			LR_LOADFROMFILE);
	GetObject(hBitmap, sizeof(BITMAP), &bm);

	hMemDC = CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
	BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	COLORREF crOld = SetTextColor(hDC, RGB(255,0,0));
	hFont = CreateFont(30, 0, //楂樺害20, 瀹藉彇0琛ㄧず鐢辩郴缁熼�鎷╂渶浣冲�
			0, 0, //鏂囨湰鍊炬枩锛屼笌瀛椾綋鍊炬枩閮戒负0
			FW_HEAVY, //绮椾綋
			0, 0, 0, //闈炴枩浣擄紝鏃犱笅鍒掔嚎锛屾棤涓垝绾�
			GB2312_CHARSET, //瀛楃闆�
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, //涓�郴鍒楃殑榛樿鍊�
			DEFAULT_PITCH | FF_DONTCARE, "瀹嬩綋" //瀛椾綋鍚嶇О
			);

	SelectObject(hDC, hFont);
	SetBkMode(hDC, TRANSPARENT);
	if (section == "app") {
		vector<string> time = Util::Split(Util::CurrentDate(), ';');
		TextOut(hDC, 685, 498, (time.at(0)).c_str(), 4); //年
		TextOut(hDC, 710, 543, (time.at(1)).c_str(), 2); //月
		TextOut(hDC, 710, 588, (time.at(2)).c_str(), 2); //日
		TextOut(hDC, 758, 638, (Config::TheDayOfWeek(time.at(3))).c_str(), 2); //星期

	}
	DrawComplete(section.c_str(), hDC);
	SetTextColor(hDC, crOld);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);
}

void DrawRate::DrawComplete(string lpAppName, HDC hdc) {

	unsigned int func_rtn;
	char StrBuf[1256];
	char work[256] = "Can not fined";
	unsigned int Count = 0;

	memset(StrBuf, 0, sizeof(StrBuf));
	memset(work, 0, sizeof(work));

	func_rtn = GetPrivateProfileSection(lpAppName.c_str(), StrBuf, 1256,
			desfile.c_str());

	while (Count < func_rtn) {
		memset(work, 0, sizeof(work));/*work鍒濇湡鍖*/
		memcpy(work, &StrBuf[Count], strlen(&StrBuf[Count]));
		Count += (strlen(&StrBuf[Count]) + 1);/**/
		vector<string> temp = Util::Split(work, '=');
		string nametype = temp.at(0);
		string rate = MediaApplication::rateInfo->Get(nametype);
		int desx = this->GetRateDesXY(lpAppName, nametype, 0);
		int desy = this->GetRateDesXY(lpAppName, nametype, 1);
		TextOut(hdc, desx, desy, rate.c_str(), strlen(rate.c_str())); //瀛椾綋杈撳嚭
	}

}

int DrawRate::GetRateDesXY(string lpAppName, string nametype, int x) {
	string des = Util::GetIniString(lpAppName, nametype, " ", desfile.c_str());
	vector<string> tempdes = Util::Split(des, ';');
	return atoi((tempdes.at(x)).c_str());

}

void DrawRate::DrawPartRate(string bmpname, string part, int interval) {
	string bmpfile = Config::InstallPath + bmpname;
	this->SetBmpFile(bmpfile);
	this->SetSection(part);
	this->Refresh();
	Sleep(1000 * interval);
}

void DrawRate::RunDrawRate() {
	interval = MediaApplication::rateInfo->GetInteger("interval", 5000);
	this->DrawPartRate("complete.bmp", "app", interval);
	this->DrawPartRate("lumppart.bmp", "lump", interval);
	this->DrawPartRate("edupart.bmp", "edu", interval);
	this->DrawPartRate("shortpart.bmp", "short", interval);
}

bool DrawRate::Do() {
	int pauseminute = MediaApplication::ratefrequency->GetInteger("frequency",
			5);
	Sleep(60 * 1000 * pauseminute);

	string isempty = MediaApplication::rateInfo->Get("current", "");
	if (isempty != "" && MediaApplication::canshowrate) {
		this->SetVisible(true);
		this->SetTopMost(true);
		this->RunDrawRate();
		this->SetVisible(false);
		return true;
	}
	return false;
}

bool DrawRate::BeforeRun() {
	this->Create();
	Sleep(400);
	this->Show();
	return true;
}

bool DrawRate::AfterRun() {
	this->Close();
	return true;
}

