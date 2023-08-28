#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <io.h> 
#include<Windows.h>
#include<string>
#include<iostream>
#include<fstream>
#include<tchar.h>
#pragma comment(lib,"URlmon.lib")

#undef UNICODE
#undef _UNICODE

using namespace std;

string getselfpathA() {
	char runpath[MAX_PATH];
	GetModuleFileNameA(NULL, runpath, MAX_PATH);

	(strrchr(runpath, '\\'))[0] = 0;

	string runpaths = runpath;
	return runpaths;
}

bool existcheckA(string& name) {
	ifstream f(name.c_str());
	return f.good();
}

bool existfileA(string file) {
	bool retEF = existcheckA(file);
	return retEF;
}

bool existfolderA(string name) {
	string A = name + "\\existfoldertest.bin";

	ofstream opentest;
	opentest.open(A);
	opentest << "writeTest" << endl;
	opentest.close();

	bool retb = existcheckA(A);
	remove(A.c_str());

	return retb;
}

string cmarkfileA(string filename, string info) {
	ofstream tempwrite;
	tempwrite.open(filename);
	tempwrite << info << endl;
	tempwrite.close();
	return info;
}

string readengineA(string file, string head, string title) {
	LPTSTR lpPath = new char[MAX_PATH];

	strcpy(lpPath, file.c_str());
	LPTSTR ReadPointWM = new char[99];

	bool existf = existcheckA(file);
	if (existf) {
		GetPrivateProfileString(head.c_str(), title.c_str(), NULL, ReadPointWM, 99, lpPath);
		return ReadPointWM;
	}
	else
	{
		string Text = "read.data.error:file not found";
		return Text;
	}
}

string readiniA(string file, string head, string title) {
	string returncheck = readengineA(file, head, title);
	if (returncheck == "") {
		string charcode = "readini-failed";
		return charcode;
	}
	return returncheck;
}

string writeiniA(string file, string head, string title, string info) {
	LPTSTR lpPath = new char[MAX_PATH];
	strcpy(lpPath, file.c_str());
	WritePrivateProfileString(head.c_str(), title.c_str(), info.c_str(), lpPath);
	string n = "succ";
	return n;
}


// real CMD

string CleanGlobal() {
	string Temp = getenv("temp");
	string WorkPath = Temp + "\\GlobalVar.gla";

	bool wkf = existfileA(WorkPath);
	if (wkf) {}
	else {
		string INFO = "notneed";
		return INFO;
	}
	
	remove(WorkPath.c_str());

    wkf = existfileA(WorkPath);
	if (wkf) {
		string INFO = "failed";
		return INFO;
	}
	else {
		string INFO = "succeed";
		return INFO;
	}
}

string WriteGlobal(string VarName,string NewSet) {
	string Temp = getenv("temp");
	string WorkPath = Temp + "\\GlobalVar.gla";

	writeiniA(WorkPath, "VarList", VarName, NewSet);

	string INFO = "succeed";
	return INFO;
}

string GetGlobal(string VarName) {
	string Temp = getenv("temp");
	string WorkPath = Temp + "\\GlobalVar.gla";

	string VarData;

	VarData = readiniA(WorkPath, "VarList", VarName);

	return VarData;
}