//Copyright FoxaXu
// API For 7zip Tools https://www.7-zip.org/

#include<Windows.h>
#pragma comment(lib,"URlmon.lib")
using namespace std;

//7z Core
// selectoption = Select ZIP Pack

string n7zNewZIP(string n7zcorex, string selectoption, string outfile, string password) {
	string SERunFile = "\"" + n7zcorex + "\"";
	string SEExtraFile = "\"" + selectoption + "\"";
	string SEOutPath = "\"" + outfile + "\"";
	if (_access(n7zcorex.c_str(), 00)){
		string xinfo = "7-Zip-API错误，请求的函数 n7zcorex 不应该为" + n7zcorex + "，无法调用对应程序";
		MessageBox(0, xinfo.c_str(), "7zipAPI Error", MB_OK);
		return n7zcorex;
	}
	if (password == "0") {
		string n7zParameter = "a " + SEOutPath + " " + SEExtraFile + " ";
		ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		string INFOS = "succeed";
		return INFOS;
	}
	else {
		string n7zParameter = "a " + SEOutPath + " " + SEExtraFile + " -p" + password;
		ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		string INFOS = "succeed";
		return INFOS;
	}
}

string n7zUNZIP(string n7zcorex, string selectoption, string outpath, string password) {
	string SERunFile = "\"" + n7zcorex + "\"";
	string SEExtraFile = "\"" + selectoption + "\"";
	string SEOutPath = "\"" + outpath + "\"";
	if (_access(n7zcorex.c_str(), 00)) {
		string xinfo = "7-Zip-API错误，请求的函数 n7zcorex 不应该为" + n7zcorex + "，无法调用对应程序";
		MessageBox(0, xinfo.c_str(), "7zipAPI Error", MB_OK);
		return n7zcorex;
	}
	if (password == "0") {
		string n7zParameter = "x -o" + SEOutPath + " " + SEExtraFile + " ";
		ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		string INFOS = "succeed";
		return INFOS;
	}
	else {
		string n7zParameter = "x -o" + SEOutPath + " " + SEExtraFile + " -p" + password;
		ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		string INFOS = "succeed";
		return INFOS;
	}
}

string n7zGetSHA256(string n7zcorex, string file) {
	string SERunFile = "\"" + n7zcorex + "\"";
	string SESFile = "\"" + file + "\"";
	if (_access(n7zcorex.c_str(), 00)) {
		string xinfo = "7-Zip-API错误，请求的函数 n7zcorex 不应该为" + n7zcorex + "，无法调用对应程序";
		MessageBox(0, xinfo.c_str(), "7zipAPI Error", MB_OK);
		return n7zcorex;
	}
	bool ret7zrfile = existfile("report~7z.txt");
	if (ret7zrfile) {
		remove("report~7z.txt");
	}

	ofstream WritePoint;
	WritePoint.open("calc~7zsha256.bat");
	WritePoint << "@echo off" << endl;
	WritePoint << SERunFile + " h - scrcsha256 \"" + SESFile + "\" >>report~7z.txt" << endl;
	WritePoint << "del calc~7zsha256.bat" << endl;
	WritePoint << "exit" << endl;
	WritePoint.close();

	ShellExecute(0, "open", "calc~7zsha256.bat", 0, 0, SW_HIDE);

BackWaitCalcSHA:
	Sleep(500);
	bool retR7S = existfile("report~7z.txt");
	if (retR7S) {
		string SHACode = ReadText("report~7z.txt",10);
		remove("report~7z.txt");
		ofstream CleanLong;
		CleanLong.open("temp~cleankey");
		CleanLong << SHACode << endl;
		CleanLong.close();

		string RealReturn;
		ifstream ReadShort;
		ReadShort.open("temp~cleankey");
		ReadShort >> RealReturn;
		ReadShort.close();

		remove("temp~cleankey");

		return RealReturn;
	}
	else {
		goto BackWaitCalcSHA;
	}
}