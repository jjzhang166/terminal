#include "Config.h"
#include "../util/Util.h"
#include <stdio.h>
#include <windows.h>

string Config::InstallPath = "C:\\Program Files\\bank\\";
string Config::FileName = Config::InstallPath + "config.ini";

Config::Config(void) {
}

Config::~Config(void) {
}

string Config::GetHost() {
	return Util::GetIniString("bank", "host", "localhost", FileName);
}

UINT Config::GetPort() {
	return Util::GetIniInt("bank", "port", 8080, FileName);
}

UINT Config::GetUpdateInterval() {
	return Util::GetIniInt("bank", "update", 20000, FileName);
}

string Config::GetUrl() {
	return Util::GetIniString("bank", "url",
			"/bank/cn/wizool/bank/servlet/InterfaceServlet", FileName);
}

string Config::GetUuidUrl() {
	return Util::GetIniString("bank", "uuidurl",
			"/bank/cn/wizool/bank/servlet/DepartmentServlet?method=getUUIDByIp",
			FileName);
}

string Config::GetSystemDir() {
	return Util::GetIniString("bank", "systemdir", "C:\\WINDOWS\\system32",
			FileName);
}

string Config::GetRootDir() {
	return Util::GetIniString("bank", "rootdir", "D:\\ROOT", FileName);
}

string Config::GetDownloadUrl() {
	return Util::GetIniString("bank", "downloadurl", "/bank/taskes", FileName);
}

string Config::GetDownloadFileUrl() {
	return Util::GetIniString("bank", "downloadfileurl",
			"/bank/cn/wizool/bank/servlet/DocumentServlet?method=download&id=",
			FileName);
}

string Config::GetVersionUrl() {
	return Util::GetIniString("bank", "versionurl",
			"/bank/cn/wizool/bank/servlet/InterfaceServlet?method=active&id=",
			FileName);
}

string Config::GetUpdateUrl() {
	return Util::GetIniString("bank", "updateurl", "/bank/client/client.zip",
			FileName);
}

string Config::GetRegisterUrl() {
	char chrs[6];
	sprintf(chrs, "%d", Config::GetPort());
	return "http://" + Config::GetHost() + ":" + chrs
			+ Util::GetIniString("bank", "registerurl", "", FileName);
}

string Config::GetExeFile(string name) {
	return Util::GetIniString("exefile", name, "", FileName);
}

string Config::GetWindowTitle(string name) {
	return Util::GetIniString("windowtitle", name, "", FileName);
}

string Config::GetProcessName(string name) {
	return Util::GetIniString("process", name, "", FileName);
}

string Config::GetFileDir() {
	return Util::GetIniString("bank", "filedir", "D:\\TMP", FileName);
}

string Config::GetRccExamDir() {
	return Util::GetIniString("bank", "rccexamdir",
			"C:\\Program Files\\山东农信-业务技能评测\\client", FileName);
}
string Config::GetRccExamExe() {
	return Util::GetIniString("bank", "rccexamexe",
			"C:\\Program Files\\山东农信-业务技能评测\\client\\RccExam.exe", FileName);
}

string Config::GetIePar() {
	return Util::GetIniString("bank", "iepar", "http://51.64.1.98", FileName);
}

string Config::GetStartupDir() {
	return Util::GetIniString("bank", "startupdir",
			"C:\\Users\\Administrator\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup",
			FileName);
}

string Config::GetTempDir() {
	return Util::GetIniString("bank", "tempdir", "D:\\TEMP", FileName);
}

string Config::TheDayOfWeek(string dayofweek) {
	string week = Util::GetIniString("bank", "dayofweek", " ", FileName);
	vector<string> tempweek = Util::Split(week, ';');
	unsigned int weeknum = atoi(dayofweek.c_str());
	string thedayofweek = tempweek.at(weeknum);
	return thedayofweek;
}

string Config::GetDownUrl() {
	return Util::GetIniString("bank", "downurl",
			"/bank/cn/wizool/bank/servlet/InterfaceServlet?method=download&id=",
			FileName);
}

string Config::GetDownTmpFile() {
	return Util::GetIniString("bank", "downtmpdir", "D:\\DOWNTMP\\", FileName);
}

