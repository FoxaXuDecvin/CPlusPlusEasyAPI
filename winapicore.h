// Windows API Loader
// Copyright FoxaXu
// Add _CRT_SECURE_NO_WARNINGS in Setting
#pragma once

//��������Щ�귭������̳�����ĸ���API�빦��ģ��
//ֱ�ӵ��ö�Ӧģ�飬���õ������ͷ�ļ�
//���˰�
// 64λ 32λ�����ã�����32�����������ƣ�������ֵ���ܳ��� 2��31�η� (2147483648),���������ը

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

//��ȡ�ı��ĵ�API
string ReadTextA(string filename, int line)
{

	//line�������� 1 - lines
	ifstream text;
	text.open(filename, ios::in);

	vector<string> strVec;
	while (!text.eof())  //��0 - ��lines��Ӧstrvect[0] - strvect[lines]
	{
		string inbuf;
		getline(text, inbuf, '\n');
		strVec.push_back(inbuf);
	}
	return strVec[line - 1];
}

//StringתLPCWSTR

LPCWSTR stringToLPCWSTR(string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[sizeof(wchar_t) * (orig.length() - 1)];
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

//���ٴ����Ǻ��ļ�
string cmarkfile(string filename, string info) {
	ofstream tempwrite;
	tempwrite.open(filename);
	tempwrite << info << endl;
	tempwrite.close();
	return info;
}

//�ļ����ڼ�飨�Ƽ�ʹ�� _access ���棩
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

//�ļ��д��ڼ��
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

//URL�����ļ����� UrlMon��չ
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

//INI��ʽ�ļ���ȡд��
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

//CMDģ�� (���� system ������ʾ�򣬷������ؿ���̨)
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

//��ȡ��ǰ����PID����
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

//�ļ��в��� rm ɾ�� md ���� clear ����
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
 
//����ָ���ļ����������ļ� �����ļ�Ŀ¼����
//[in] const wstring wstDirectory : Ҫ������ļ�Ŀ¼
//����ֵ : ��
void rmfolderAPI(string dir) {
	string newDir = dir + "\\*.*";
	intptr_t handle;
	struct _finddata_t fileinfo;

	handle = _findfirst(newDir.c_str(), &fileinfo);
	if (handle == -1) {
		//cout << "���ļ�" << endl;
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

//�ļ��в��� rm ɾ�� md ���� clear ����
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

//�ļ��в��� rm ɾ�� md ���� clear ����
void mdfolder(string foldername) {
	CreateDirectory(foldername.c_str(), 0);
	return;
}

//�ļ��в��� rm ɾ�� md ���� clear ����
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

//��ȡ����txt��ʽ���ļ�����
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

//��ȡ��������������Ŀ¼���ļ�
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

//���Դ��κγ����API
string OpenWebsite(string URL) {
	ShellExecute(0, "open", URL.c_str(), 0, 0, SW_SHOW);
	string INFO = "succeed";
	return INFO;
}

// OS API
//�����ļ���
bool testAdmin(string TestDir) {
	string testfile = TestDir + "\\testadmin~winapicore.bin";

	cmarkfile(testfile, "testAdmin");

	bool retINFO = existfile(testfile);

	remove(testfile.c_str());

	return retINFO;
}

//��ȡ����Ŀ¼�����ļ�
string getselfpath() {
	char runpath[MAX_PATH];
	GetModuleFileNameA(NULL, runpath, MAX_PATH);

	(strrchr(runpath, '\\'))[0] = 0;

	string runpaths = runpath;
	return runpaths;
}

//�ж�ϵͳ��64λ����32λ������ֵ 32bit / 64bit
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
	ReadFile.open(filename.c_str());//ios::in ��ʾ��ֻ���ķ�ʽ��ȡ�ļ�
	if (ReadFile.fail())//�ļ���ʧ��:����0
	{
		return 0;
	}
	else//�ļ�����
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

//��ȡwindows���� (����250����0.25����ӳ٣�������������ʹ�� getenv���棩)
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

//��getenv�İ汾
string getwinenvfast(string varname) {
	string retcode = getenv(varname.c_str());
	return retcode;
}

//ʹ��ϵͳAPI��ȡ��������
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

//���һ�ξ������Ƿ����ָ�����ַ����ڷ��� û�з���0���з���1
int checkChar(string text,string chechchar ) {
	const char* c = chechchar.c_str();

	if (text.find(c) != string::npos) {
		return 1;
	}
	else {
		return 0;
	}
}


//�и��ַ�����linesΪ���ݣ�cutmarkΪ��ǣ�lineΪλ��

string cutlineblockA(string lines, string cutmark, int line) {
	string backapi;
	char* readcut = NULL;

	char Texts[65535] = "a";
	char CUMark[65535] = "a";

	strcpy_s(Texts, lines.c_str());
	strcpy_s(CUMark, cutmark.c_str());

	// �ָ��ַ��������طָ������ַ���
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

//�滻��������ĵ��� ��߿����� 65535���ַ�
string Replace(string& info, const string& replaceword, const string& nword) {
	//cout << "New Replace :  _" << info << "_  _" << replaceword << "_  _" << nword << "_" << endl;
	char repinfo[65535];
	strcpy(repinfo, replaceword.c_str());
	int checkanti = checkChar(info, repinfo);
	if (checkanti == 1) {
		std::string dst_str = info;
		std::string::size_type pos = 0;
		while ((pos = dst_str.find(replaceword)) != std::string::npos)   //�滻����ָ���Ӵ�
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

//��������� (���2147483647 ��С-2147483647 ,���ڲ���bug���ڣ���ֹͣ�쳣��ֵ���)
int SpawnRandomNum(int min, int max) {

	string minb, maxb;
	minb = to_string(min);
	maxb = to_string(max);

	//string chars = "Min :  " + minb + "   Max :   " + maxb + "  Bug Report";
	//MessageBox(0, chars.c_str(), "MXBug Report", MB_OK);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max); // ����i-a֮����������
	int outdata = dis(gen);
	
	//string dis_str = to_string(dis(gen));
	//MessageBox(0,dis_str.c_str(),"Bug check",MB_OK);
	
	return dis(gen);
}

//ִ��Pause�������Ӧ��Ϣ
void cpause(string Notices) {
	cout << Notices;
	system("pause >nul");
	cout << endl;
	return;
}

