/*
 * MainClass.cpp
 *
 *  Created on: Jun 16, 2012
 *      Author: zhangbo
 */

#include "../util/Util.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

#include "../thread/FileManager.h"
#include "../util/CommandListener.h"
#include "../util/CommandReceiver.h"
#include "../util/ThreadCreator.h"
#include "../util/Logger.h"
#include "StatusReport.h"
#include <windows.h>
#include "../util/HttpClient.h"
#include "../util/Configure.h"
#include "../util/NetInformation.h"
#include "../task/DrawList.h"

class CommondExecuter: public CommandListener {
public:
	void Start() {
		RequestParameter param("64.112.8.251", 8088,
				"/bank/cn/wizool/bank/servlet/InterfaceServlet");
		param.PutString("method", "getTask");
		param.PutString("id", "4d72d011-4df3-44ff-a458-8323a89fcf82");
		CommandReceiver* receiver = new CommandReceiver(param);
		receiver->SetListener(this);
		void* h = ThreadCreator::StartThread(receiver);
		WaitForSingleObject(h, INFINITE);
	}

	void Receive(const string& command) {
		cout << command << endl;
	}
};

void Download() {
	Logger::SetDefault("log.txt", Logger::LOG_DEBUG);
	FileManager fileManager;
	fileManager.SetDirectory("C:/TMP");
	fileManager.SetTempDirectory("C:/DOWNTMP");
	RequestParameter param("192.168.1.115", 8888,
			"/bank/cn/wizool/bank/servlet/InterfaceServlet");
	param.PutString("id", "dcecac57-18bc-4fa1-8a20-1f905ee8da12");
	param.PutString("method", "download");
	fileManager.SetRequestParameter(param);
	fileManager.DownFile("a94b54aa4a79e4f767de116b1b77473d");
}

string SystemUUID() {
	string uuidFile = "./uuid.txt";
	string uuid;
	srand((unsigned) time(NULL));
	while (true) {
		if (Util::Download("64.112.8.251",
				"/bank/cn/wizool/bank/servlet/DepartmentServlet?method=getUUIDByIp",
				80, uuidFile)) {
			uuid = Util::ReadFile(uuidFile, 36);
			if (uuid.size() == 36) {
				break;
			}
		}
		Sleep(rand() % 3000);
	}
	return uuid;
}

int main(int argc, char* argv[]) {

//	CommondExecuter* ce = new CommondExecuter;
//	ce->Start();

//	string names = "张波.mp4;王燕.mp4;";
//	DrawList::ShowList(Util::Split(names, ';'), 10);

//	Util::ExecuteCommand("dir", "C:\\Program Files\\bank", "c:\\command.txt");
//
//	Sleep(5000);
//	Util::SaveScreen(file);
//	HttpClient c("64.112.8.251", 8088, "/bank/cn/wizool/bank/servlet/InterfaceServlet");
//	c.SetFile(file);
//	c.SetId("4d72d011-4df3-44ff-a458-8323a89fcf82");
//	c.Post();

//	Configure::SetDefault("C:\\program files\\bank\\a.txt");
//	cout << "<" << Configure::Default()->Get("taskName") << ">" << endl;
//	cout << "<" << Configure::Default()->Get("fileName") << ">" << endl;
//	cout << "<" << Configure::Default()->Get("names") << ">" << endl;

//	NetInformation info("64.112.8.251", 8088,
//			"/bank/cn/wizool/bank/servlet/InterfaceServlet?id=067c43da-8b28-44c8-beda-be623b1120b9&method=getDesc&tid=6fc8c5f5-bad7-44a1-a539-28fccaee3b1f",
//			4096);
//	info.Update();
//	cout << "<" << info.Get("taskName") << ">" << endl;
//	cout << "<" << info.Get("fileName") << ">" << endl;
//	cout << "<" << info.Get("names") << ">" << endl;
//
//	cout << "<" << Util::Trim("\r\nnames 	") << ">" << endl;
//	cout << "<" << Util::Trim("\t\r \nnames 	") << ">" << endl;
//	cout << "<" << Util::Trim("\t\r \n 	") << ">" << endl;

	Download();
//	RequestParameter param("64.112.8.10", 80,
//			"/bank/cn/wizool/bank/servlet/InterfaceServlet", 1000);
//	param.PutString("method", "getDesc");
//	param.PutString("id", "067c43da-8b28-44c8-beda-be623b1120b9");
//	param.PutString("tid", "067c43da-8b28-44c8-beda-be623b1120b9");
//	HttpRequest::Create(param).Get(1);
	return (0);
}
