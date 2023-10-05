// Windows API Loader
// Copyright FoxaXu
// Add _CRT_SECURE_NO_WARNINGS in Setting
#pragma once

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
#include<stdio.h>
#include<xstring>
#include<cstring>
#include<vector>
#include<tchar.h>
#include<direct.h>
#include <shellapi.h>
#include <urlmon.h>

#pragma comment(lib,"URlmon.lib")

#undef UNICODE
#undef _UNICODE
#define BUFSIZE 4096
using std::cout; using std::cin;
using std::endl; using std::string;
using namespace std;

int charlong;

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

bool existfolderA(string name,string markfile) {
	string A = name + "\\" + markfile;

	bool retb = existcheck(A);

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
	LPTSTR lpPath = new char[65535];

	strcpy(lpPath, file.c_str());
	LPTSTR ReadPointWM = new char[65535];

	bool existf = existcheck(file);
	if (existf) {
		GetPrivateProfileString(head.c_str(), title.c_str(), NULL, ReadPointWM, 65535, lpPath);
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
	char	fileName[65535] = { 0 };
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
string oldrmfolder(string foldername) {
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
 
//清理指定文件夹下所有文件 包括文件目录本身
//[in] const wstring wstDirectory : 要清理的文件目录
//返回值 : 无
void rmfolderAPI(string dir) {
	string newDir = dir + "\\*.*";
	intptr_t handle;
	struct _finddata_t fileinfo;

	handle = _findfirst(newDir.c_str(), &fileinfo);
	if (handle == -1) {
		//cout << "无文件" << endl;
		system("pause");
		return;
	}

	do {
		if (fileinfo.attrib & _A_SUBDIR) {
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;
			newDir = dir + "\\" + fileinfo.name;
			rmfolderAPI(newDir.c_str());
			//cout << newDir.c_str() << endl;
			if (_rmdir(newDir.c_str()) == 0) {
				//cout << "delete empty dir success" << endl;
				RemoveDirectory(newDir.c_str());
			}
			else {
				//cout << "delete empty dir error" << endl;
			}
		}
		else {
			string file_path = dir + "\\" + fileinfo.name;
			//cout << file_path.c_str() << endl;
			if (remove(file_path.c_str()) == 0) {
				//cout << "delete file success" << endl;
			}
			else {
				//cout << "delete file error" << endl;
			}
		}
	} while (!_findnext(handle, &fileinfo));

	_findclose(handle);
}

//文件夹操作 rm 删除 md 创建 clear 清理
void rmfolder(string folderpath) {
	rmfolderAPI(folderpath);
	RemoveDirectory(folderpath.c_str());
	return;
}

void rmfolderOLD(string foldername) {
	string cmde = "rd /s /q \"" + foldername + "\"";
	system(cmde.c_str());
	return;
}

//文件夹操作 rm 删除 md 创建 clear 清理
void mdfolder(string foldername) {
	CreateDirectory(foldername.c_str(), 0);
	return;
}

//文件夹操作 rm 删除 md 创建 clear 清理
void clearfolder(string foldername) {
	string cmdsa = "rd /s /q \"" + foldername + "\"";
	system(cmdsa.c_str());
	CreateDirectory(foldername.c_str(),0);
	return;
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
	string strVec[65535];
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
	string b = getenv("temp");
	string charr = b + "\\guc~temp.txt";
	URLDown(url, charr);
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
	char	fileName[65535] = { 0 };
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

int CountLines(string filename)
{
	ifstream ReadFile;
	int n = 0;
	string tmp;
	ReadFile.open(filename.c_str());//ios::in 表示以只读的方式读取文件
	if (ReadFile.fail())//文件打开失败:返回0
	{
		return 0;
	}
	else//文件存在
	{
		while (getline(ReadFile, tmp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
}

string ALineReader(string File, int line_number) {
	int lines, i = 0;
	string temp;
	fstream file;
	file.open(File.c_str());
	lines = CountLines(File);

	if (line_number <= 0)
	{
		return "Line Error";
	}
	if (file.fail())
	{
		return "Error File not exist";
	}
	if (line_number > lines)
	{
		return "overline";
	}
	while (getline(file, temp) && i < line_number - 1)
	{
		i++;
	}
	file.close();
	return temp;
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
	string RETd =ALineReader("varout.txt", 1);
	remove("sc.bat");
	remove("varout.txt");

	return RETd;
}

//用getenv的版本
string getwinenvfast(string varname) {
	string retcode = getenv(varname.c_str());
	return retcode;
}

//使用系统API获取环境变量
string getwinenvapi(string varname) {

	char buffer[65535];
	DWORD ret;
	ret = GetEnvironmentVariable(varname.c_str(), buffer, 32767);

	string back = buffer;
	if (back == "") {
		string cur_str = to_string(long long(GetLastError()));
		return cur_str;
	}
	return back;
}

//检查一段句子中是否存在指定的字符存在返回 没有返回0，有返回1
int checkChar(string text,string chechchar ) {
	const char* c = chechchar.c_str();

	if (text.find(c) != string::npos) {
		return 1;
	}
	else {
		return 0;
	}
}


//切割字符串。lines为内容，cutmark为标记，line为位数

string cutlineblockA(string lines, string cutmark, int line) {
	string backapi;
	char* readcut = NULL;

	char Texts[65535] = "a";
	char CUMark[65535] = "a";

	strcpy_s(Texts, lines.c_str());
	strcpy_s(CUMark, cutmark.c_str());

	// 分割字符串，返回分割后的子字符串
	int cutrecord = 1;
	char* token = strtok_s(Texts, CUMark, &readcut);
	if (token == NULL) {
		backapi = "";
		return backapi;
	}

	if (cutrecord == line) {

		//cout << "CUTLINEBLOCK CHECK OK, RETURN :  _" << token << "_" << endl;
		backapi = token;
		return backapi;
	}

NextRollCR:
	if (cutrecord == line) {
		//cout << "CUTLINEBLOCK CHECK OK, RETURN :  _" << token << "_" << endl;
		backapi = token;
		return backapi;
	}
	if (token == NULL) {
		backapi = "";
		return backapi;
	}
	cutrecord++;
	token = strtok_s(NULL, CUMark, &readcut);
	goto NextRollCR;
}

string cutlineblock(string lines, int line) {
	return cutlineblockA(lines, " ", line);
}

//替换句子里面的单词 最高可容纳 65535个字符
string Replace(string& info, const string& replaceword, const string& nword) {
	//cout << "New Replace :  _" << info << "_  _" << replaceword << "_  _" << nword << "_" << endl;
	char repinfo[65535];
	strcpy(repinfo, replaceword.c_str());
	int checkanti = checkChar(info, repinfo);
	if (checkanti == 1) {
		std::string dst_str = info;
		std::string::size_type pos = 0;
		while ((pos = dst_str.find(replaceword)) != std::string::npos)   //替换所有指定子串
		{
			dst_str=dst_str.replace(pos, replaceword.length(), nword);
			if (pos > 2000) {
				MessageBox(0, "Replace API is timeout", "Error", MB_ICONERROR | MB_OK);
				return "replaceError";
			}
		}
		//cout << "Return Data :  _" << dst_str <<"_" << endl;
		return dst_str;
	}
	else {
		//cout << "Return Data :  _" << info << "_" << endl;
		return info;
	}
}

//创建随机数 (最大2147483647 最小-2147483647 ,由于部分bug存在，已停止异常数值检查)
int SpawnRandomNum(int min, int max) {

	string minb, maxb;
	minb = to_string(min);
	maxb = to_string(max);

	//string chars = "Min :  " + minb + "   Max :   " + maxb + "  Bug Report";
	//MessageBox(0, chars.c_str(), "MXBug Report", MB_OK);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max); // 生成i-a之间的随机整数
	int outdata = dis(gen);
	
	//string dis_str = to_string(dis(gen));
	//MessageBox(0,dis_str.c_str(),"Bug check",MB_OK);
	
	return dis(gen);
}

//执行Pause，输出对应信息
void cpause(string Notices) {
	cout << Notices;
	system("pause >nul");
	cout << endl;
	return;
}

