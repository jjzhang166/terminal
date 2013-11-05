#pragma once
#include <string>
#include <vector>
using namespace std;

class Config {
private:
	static string FileName;

public:
	Config(void);
	~Config(void);

	static string InstallPath;

	static string GetHost();

	static unsigned int GetPort();

	static unsigned int GetUpdateInterval();

	static string GetUrl();

	static string GetUuidUrl();

	static string GetSystemDir();

	static string GetRootDir();

	static string GetFileDir();

	static string GetDownloadUrl();

	static string GetVersionUrl();

	static string GetUpdateUrl();

	static string GetRegisterUrl();

	static string GetExeFile(string name);

	static string GetWindowTitle(string name);

	static string GetProcessName(string name);

	static string GetDownloadFileUrl();

	static string GetDescIniFile();

	static string GetRccExamDir();

	static string GetRccExamExe();

	static string GetIePar();

	static string GetStartupDir();

	static string GetTempDir();

	static string TheDayOfWeek(string dayofweek);

	static string GetDownUrl();

	static string GetDownTmpFile();

};

