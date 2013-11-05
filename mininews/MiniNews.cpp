// MiniNews.cpp : 定义应用程序的入口点。
//

//#include "resources/stdafx.h"
#include "MiniNews.h"
#include "NewsWindow.h"
#include "message.h"
//#include "resources/resource.h"
#include "ShowChange.h"

CNewsWindow NewsWindow;
BOOL InitNews(char *details);

//int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
//{
////	ShowChange new1("xiahui");
////	if(!new1.InitNews())
////		return 0;
////	Sleep(5000);
////	ShowChange new2("11");
////	if(!new2.InitNews())
////		return 0;
//	if(!InitNews("123"))
//		return 0;
//
////	 主消息循环:
////	MSG msg;
////	while (GetMessage(&msg, NULL, 0, 0))
////	{
////		TranslateMessage(&msg);
////		DispatchMessage(&msg);
////	}
////
////	return (int) msg.wParam;
//}
BOOL InitNews(char *details) {
	LPCTSTR lpNewsTitle = title;
	LPCTSTR lpNewsContent = details;
	LPCTSTR lpNewsURL = ("http://www.baidu.com/");

	NewsWindow.SetSkin(MAKEINTRESOURCE(IDB_SKIN_QQ));
	if (!NewsWindow.Create(name))
		return FALSE;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
	return TRUE;
}
