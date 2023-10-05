//Copyright FoxaXu
// API For 7zip Tools https://www.7-zip.org/

#include<Windows.h>
#pragma comment(lib,"URlmon.lib")
using namespace std;

//7z Core
// selectoption = Select ZIP Pack

void ECMD(string ncmd) {
	string cmd = "\"" + ncmd + "\" 1>nul >nul";
	//cout << cmd << endl;
	system(cmd.c_str());
	return;
}

void n7zNewZIP(string n7zcorex, string selectoption, string outfile, string password) {
	string SERunFile = "\"" + n7zcorex + "\"";
	string SEExtraFile = "\"" + selectoption + "\"";
	string SEOutPath = "\"" + outfile + "\"";
	if (_access(n7zcorex.c_str(), 00)){
		string xinfo = "7-Zip-API��������ĺ��� n7zcorex ��Ӧ��Ϊ" + n7zcorex + "���޷����ö�Ӧ����";
		MessageBox(0, xinfo.c_str(), "7zipAPI Error", MB_OK);
		Sleep(200);
		return;
	}
	if (password == "0") {
		string n7zParameter = "a " + SEOutPath + " " + SEExtraFile + " ";
		//ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		ECMD(SERunFile + " " + n7zParameter);
		string INFOS = "succeed";
		Sleep(200);
		return;
	}
	else {
		string n7zParameter = "a " + SEOutPath + " " + SEExtraFile + " -p" + password;
		//ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		ECMD(SERunFile + " " + n7zParameter);
		string INFOS = "succeed";
		Sleep(200);
		return;
	}
}

void n7zUNZIP(string n7zcorex, string selectoption, string outpath, string password) {
	string SERunFile = "\"" + n7zcorex + "\"";
	string SEExtraFile = "\"" + selectoption + "\"";
	string SEOutPath = "\"" + outpath + "\"";
	if (_access(n7zcorex.c_str(), 00)) {
		string xinfo = "7-Zip-API��������ĺ��� n7zcorex ��Ӧ��Ϊ" + n7zcorex + "���޷����ö�Ӧ����";
		MessageBox(0, xinfo.c_str(), "7zipAPI Error", MB_OK);
		Sleep(200);
		return;
	}
	if (password == "0") {
		string n7zParameter = "x -o" + SEOutPath + " " + SEExtraFile + " ";
		//ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		//cout << SERunFile + " " + n7zParameter << endl;
		ECMD(SERunFile + " " + n7zParameter);
		string INFOS = "succeed";
		Sleep(200);
		return;
	}
	else {
		string n7zParameter = "x -o" + SEOutPath + " " + SEExtraFile + " -p" + password;
		//ShellExecute(0, "open", SERunFile.c_str(), n7zParameter.c_str(), 0, SW_HIDE);
		ECMD(SERunFile + " " + n7zParameter);
		string INFOS = "succeed";
		Sleep(200);
		return;
	}
}

string n7zGetSHA256(string n7zcorex, string file) {
	string SERunFile = "\"" + n7zcorex + "\"";
	string SESFile = "\"" + file + "\"";
	if (_access(n7zcorex.c_str(), 00)) {
		string xinfo = "7-Zip-API��������ĺ��� n7zcorex ��Ӧ��Ϊ" + n7zcorex + "���޷����ö�Ӧ����";
		MessageBox(0, xinfo.c_str(), "7zipAPI Error", MB_OK);
		Sleep(200);
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
		Sleep(200);

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