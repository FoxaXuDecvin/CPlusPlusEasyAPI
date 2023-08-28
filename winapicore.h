// Windows API Loader
// Copyright FoxaXu
// Add _CRT_SECURE_NO_WARNINGS in Setting

//这是我这些年翻各种论坛制作的各种API与功能模块
//直接调用对应模块，不用单独添加头文件
//懒人包
// 64位 32位都能用，但是32会有整数限制，所有数值不能超过 2的31次方 (2147483648),超过程序会炸

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <io.h> 
#include<Windows.h>
#include<string>
#include<iostream>
#include<fstream>
#include<tchar.h>
#include<vector>
#include<random>
#pragma comment(lib,"URlmon.lib")

#undef UNICODE
#undef _UNICODE
using std::cout; using std::cin;
using std::endl; using std::string;
using namespace std;

//读取文本文档API
string ReadTextA(string filename, int line)
{

	//line行数限制 1 - lines
	ifstream text;
	text.open(filename, ios::in);

	vector<string> strVec;
	while (!text.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
	{
		string inbuf;
		getline(text, inbuf, '\n');
		strVec.push_back(inbuf);
	}
	return strVec[line - 1];
}

//String转LPCWSTR

LPCWSTR stringToLPCWSTR(string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[sizeof(wchar_t) * (orig.length() - 1)];
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

//快速创建记号文件
string cmarkfile(string filename, string info) {
	ofstream tempwrite;
	tempwrite.open(filename);
	tempwrite << info << endl;
	tempwrite.close();
	return info;
}

//文件存在检查（推荐使用 _access 代替）
bool existcheck(string& name) {
	ifstream f(name.c_str());
	return f.good();
}

bool NewFormatExitfile(string file_or_path) {

	string realcmds = "\"" + file_or_path + "\"";

	string fileload = realcmds;

	cmarkfile("test.bin", "HELLOWORLD");
	string charcmd = "test.bin";
	if (_access(fileload.c_str(), 0)) {
		bool around = existcheck(charcmd);
		remove(charcmd.c_str());
		return around;
	}
	else {
		string abc = "1145141919810.binNNNNNNN";
		bool around = existcheck(abc);
		remove(charcmd.c_str());
		return around;
	}
}

bool existfile(string file) {
	bool retEF = existcheck(file);
	return retEF;
}

//文件夹存在检查
bool existfolder(string name) {
	string A = name + "\\existfoldertest.bin";
	
	ofstream opentest;
	opentest.open(A);
	opentest << "writeTest" << endl;
	opentest.close();

	bool retb = existcheck(A);
	remove(A.c_str());

	return retb;
}

//URL网络文件下载 UrlMon扩展
bool URLDown(string URL, string SavePath) {
	bool retDLA = existcheck(SavePath);
	if (retDLA) {
		remove(SavePath.c_str());
	}

	LPCWSTR LcDsp = stringToLPCWSTR(SavePath);
	LPCWSTR LcURL = stringToLPCWSTR(URL);
	HRESULT applydownload = URLDownloadToFileW(
		nullptr,
		LcURL,
		LcDsp,
		0,
		nullptr
	);
	bool retDLB = existfile(SavePath);
	return retDLB;
}

//INI格式文件读取写入
string readengine(string file, string head, string title) {
	LPTSTR lpPath = new char[MAX_PATH];

	strcpy(lpPath, file.c_str());
	LPTSTR ReadPointWM = new char[99];

	bool existf = existcheck(file);
	if (existf) {
		GetPrivateProfileString(head.c_str(), title.c_str(), NULL, ReadPointWM, 99, lpPath);
		return ReadPointWM;
	}
	else
	{
		string Text = "readini-failed";
		return Text;
	}
}

string readini(string file, string head, string title) {
	string returncheck = readengine(file, head, title);
	if (returncheck == "") {
		string charcode = "readini-failed";
		return charcode;
	}
	return returncheck;
}

string writeini(string file, string head, string title,string info) {
	LPTSTR lpPath = new char[MAX_PATH];
	strcpy(lpPath, file.c_str());
	WritePrivateProfileString(head.c_str(), title.c_str(), info.c_str(), lpPath);
	string n = "succ";
	return n;
}

//CMD模拟 (类似 system 不带显示框，方便隐藏控制台)
string cmdshell(string command,string runpath,string taskcode) {
	ofstream ShellOutput;
	string code = "$temp" + taskcode + "CMD.bat";
	ShellOutput.open(code);
	ShellOutput << "set oldcd=%cd%" << endl;
	ShellOutput << "cd /d " << runpath << endl;
	ShellOutput << command << endl;
	ShellOutput << "cd /d %oldcd%" << endl;
	ShellOutput << "del /q " + code << endl;
	ShellOutput << "exit" << endl;
	ShellOutput.close();

	ShellExecute(0, "open", code.c_str(), 0, 0, SW_HIDE);
	return code;
}

string cmdshelladmin(string command, string runpath, string taskcode) {
	ofstream ShellOutput;
	string code = "$temp" + taskcode + "CMD.bat";
	ShellOutput.open(code);
	ShellOutput << "set oldcd=%cd%" << endl;
	ShellOutput << "cd /d " << runpath << endl;
	ShellOutput << command << endl;
	ShellOutput << "cd /d %oldcd%" << endl;
	ShellOutput << "del /q " + code << endl;
	ShellOutput << "exit" << endl;
	ShellOutput.close();

	ShellExecute(0, "runas", code.c_str(), 0, 0, SW_HIDE);
	return code;
}

//获取当前程序PID号码
int getpid() {
	errno_t	err = 0;
	char	fileName[100] = { 0 };
	char    ProcessFullName[_MAX_PATH] = { 0 };
	char    ProcessName[0x40] = { 0 };
	DWORD   ProcessPID = 0;
	char* tmp1 = NULL;
	char* tmp2 = NULL;

	ProcessPID = GetCurrentProcessId();
	GetModuleFileNameA(NULL, ProcessFullName, _MAX_PATH);

	tmp1 = strrchr((char*)ProcessFullName, '\\');
	tmp2 = strrchr((char*)ProcessFullName, '.');
	memcpy(ProcessName, tmp1 + 1, min(tmp2 - tmp1 - 1, 0x40));
	return ProcessPID;
}

//文件夹操作 rm 删除 md 创建 clear 清理
string rmfolder(string foldername) {
	int PID = getpid();

	string PIDS = to_string(PID);

	ofstream RMTask;
	string filename = "$task~rmfolder~" + PIDS + ".bat";
	RMTask.open(filename);
	RMTask << "@echo off" << endl;
	RMTask << "rd /s /q " + foldername << endl;
	RMTask << "del " + filename << endl;
	RMTask << "exit" << endl;
	RMTask.close();

	ShellExecute(0, "open", filename.c_str(), 0, 0, SW_HIDE);
	return filename;
}

string mdfolder(string foldername) {
	string name = foldername;
	CreateDirectory(foldername.c_str(), 0);
	return name;
}

string clearfolder(string foldername) {

	int PID = getpid();

	string PIDS = to_string(PID);

	ofstream RMTask;
	string filename = "$task~rmfolder~" + PIDS + ".bat";
	RMTask.open(filename);
	RMTask << "@echo off" << endl;
	RMTask << "rd /s /q \"" + foldername + "\"" << endl;
	RMTask << "md \"" + foldername + "\"" << endl;
	RMTask << "del " + filename << endl;
	RMTask << "exit" << endl;
	RMTask.close();

	ShellExecute(0, "open", filename.c_str(), 0, 0, SW_HIDE);
	return filename;
}

//Read info

string ReadText(string filename, int line)
{
	if (_access(filename.c_str(), 0)) {}
	else {
		MessageBox(0, "Program Load Error :  NULL File Read", "ReadText", MB_OK);
		string OEM = "NULL";
		return OEM;
	}
	ifstream fin;
	fin.open(filename, ios::in);
	string strVec[6400];
	int i = 0;
	while (!fin.eof())
	{
		string inbuf;
		getline(fin, inbuf, '\n');
		strVec[i] = inbuf;
		i = i + 1;
	}
	return strVec[line - 1];
}

//获取网络txt格式的文件内容
string geturlcode(string url) {
	URLDown(url, "guc~temp.txt");
	string charr = "guc~temp.txt";
	bool retGUC = existcheck(charr);
	if (retGUC) {
		ifstream ReadAPI;
		ReadAPI.open(charr.c_str());
		string backchar;
		ReadAPI >> backchar;
		ReadAPI.close();

		remove(charr.c_str());

		return backchar;
	}
	else {
		string rechar = "geturlfailed";
		return rechar;
	}
}

//获取程序完整的运行目录加文件
string getselfinfo() {
	errno_t	err = 0;
	char	fileName[100] = { 0 };
	char    ProcessFullName[_MAX_PATH] = { 0 };
	char    ProcessName[0x40] = { 0 };
	DWORD   ProcessPID = 0;
	char* tmp1 = NULL;
	char* tmp2 = NULL;

	ProcessPID = GetCurrentProcessId();
	GetModuleFileNameA(NULL, ProcessFullName, _MAX_PATH);

	tmp1 = strrchr((char*)ProcessFullName, '\\');
	tmp2 = strrchr((char*)ProcessFullName, '.');
	memcpy(ProcessName, tmp1 + 1, min(tmp2 - tmp1 - 1, 0x40));

	return ProcessFullName;
}

//COUT

//可以打开任何程序的API
string OpenWebsite(string URL) {
	ShellExecute(0, "open", URL.c_str(), 0, 0, SW_SHOW);
	string INFO = "succeed";
	return INFO;
}

// OS API
//还是文件夹
bool testAdmin(string TestDir) {
	string testfile = TestDir + "\\testadmin~winapicore.bin";

	cmarkfile(testfile, "testAdmin");

	bool retINFO = existfile(testfile);

	remove(testfile.c_str());

	return retINFO;
}

//获取运行目录不带文件
string getselfpath() {
	char runpath[MAX_PATH];
	GetModuleFileNameA(NULL, runpath, MAX_PATH);

	(strrchr(runpath, '\\'))[0] = 0;

	string runpaths = runpath;
	return runpaths;
}

//判断系统是64位还是32位，返回值 32bit / 64bit
string getsysarch() {
	if (_access("C:\\Windows\\SysWOW64", 0)){
		string OSBIT = "32bit";
		return OSBIT;
	}
	else {
		string OSBIT = "64bit";
		return OSBIT;
	}
}

//获取windows变量 (存在250毫秒0.25秒的延迟，如果有需求可以使用 getenv代替）)
string getwinenv(string varname) {
	ofstream OutCodeSC;
	OutCodeSC.open("sc.bat");
	OutCodeSC << "@echo off" << endl;
	OutCodeSC << "echo=%" + varname + "%>>varout.txt" << endl;
	OutCodeSC << "exit" << endl;
	OutCodeSC.close();

	ShellExecute(0, "open", "sc.bat", 0, 0, SW_HIDE);
	Sleep(250);
	ifstream GetVarfile;
	string ReturnVar;
	GetVarfile.open("varout.txt");
	GetVarfile >> ReturnVar;
	GetVarfile.close();
	remove("sc.bat");
	remove("varout.txt");

	return ReturnVar;
}

//用getenv的版本
string getwinenvfast(string varname) {
	string retcode = getenv(varname.c_str());
	return retcode;
}

//检查一段句子中是否存在指定的字符
int checkChar(string text, char* c) {
	if (text.find(c) != string::npos) {
		return 1;
	}
	else {
		return 0;
	}
}

//替换句子里面的单词
string Replace(string& info, const string& replaceword, const string& nword) {
	char repinfo[20];
	strcpy(repinfo, replaceword.c_str());
	int checkanti = checkChar(info, repinfo);
	if (checkanti == 1) {
		std::string dst_str = info;
		std::string::size_type pos = 0;
		while ((pos = dst_str.find(replaceword)) != std::string::npos)   //替换所有指定子串
		{
			dst_str.replace(pos, replaceword.length(), nword);
		}
		return dst_str;
	}
	else {
		return info;
	}
}

//创建随机数
int SpawnRandomNum(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max); // 生成1-10之间的随机整数
	return dis(gen);
}