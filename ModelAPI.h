#include"D:\CppHeader\winapicore.h"
#include"D:\CppHeader\GlobalVar.h"
using namespace std;

#define _CRT_SECURE_NO_WARNINGS

string RTCode;

string EasyIntGlobal(string GlobalData, int intdata) {
	RTCode = "notset";
	string IntNum = to_string(intdata);
	WriteGlobal(GlobalData, IntNum);
	return RTCode;
}
int GetIntGlobal(string GlobalData) {
	int RetIntd;
	string GETG = GetGlobal(GlobalData);
	RetIntd = atoi(GETG.c_str());
	return RetIntd;
}
string GetForward(string Dict,string title) {
	RTCode = readini(Dict, "XsForward", title);
	return RTCode;
}
string GetAsk(string Dict, string title) {
	RTCode = readini(Dict, "XsAsk", title);
	return RTCode;
}
int GetMaxNum(string Dict, string Head) {
	RTCode = readini(Dict, Head, "$maxnum");
	int ReturnINFO;
	ReturnINFO = atoi(RTCode.c_str());
	return ReturnINFO;
}

int existMRA(string Dict, string head,string MRAset) {
	int maxnm = GetMaxNum(Dict, head);
	WriteGlobal("existMRA~s", "1");

BackScan:
	int glg = GetIntGlobal("existMRA~s");
	if (glg > maxnm) {
		return 0;
	}
	string glg_str = to_string(glg);

	string MRALS = "MRA" + glg_str;

	string getinfo = readini(Dict, head, MRALS);
	if (getinfo == MRAset) {
		return 1;
	}
	glg++;
	EasyIntGlobal("existMRA~s", glg);
	goto BackScan;
}

int WriteMaxNum(string Dict, string Head,int maxnum) {
	string maxnumse = to_string(maxnum);
	writeini(Dict, Head, "$maxnum",maxnumse);
	return maxnum;
}
string WriteNewMRA(string Dict, string Head, string INFO) {
	int getmaxnum = GetMaxNum(Dict, Head);
	getmaxnum++;
	WriteMaxNum(Dict, Head, getmaxnum);
	string gmnstr = to_string(getmaxnum);
	string NewMRA = "MRA" + gmnstr;
	writeini(Dict, Head, NewMRA, INFO);
	return NewMRA;
}
string AutoGetWordX(string Dict, string TypeIN) {
	//cout << "User Type :  _" << TypeIN << "_ ." << endl;

	int maxnums = GetMaxNum(Dict, "XsForward");
	EasyIntGlobal("Xs~WordScanAPINum", 1);

	string defaultAPI = "xs~WordScanAPI.Get";

	CleanGlobal();

	string ScanOut,sfile,TestRet;
	int RetWord;
	
ReScan:
	int nownum = GetIntGlobal("Xs~WordScanAPINum");
	if (nownum > maxnums) {
		//扫描完成，汇总结果
		string GetSelfScan = GetGlobal(defaultAPI);
		//cout << "Word Scan API return :   _" << GetSelfScan << "_ ." << endl;
		if (GetSelfScan == "readini-failed") {
			RTCode = "nullword";
			return RTCode;
		}
		return GetSelfScan;
	}

	string nownum_str = to_string(nownum);
	sfile = "MRA" + nownum_str;
	
	TestRet = readini(Dict, "XsForward", sfile);
	//cout << "scan head :  " << "XsForward  " << "Scan Title :   _" << sfile << "_ ." << endl;
	//cout << "get word num :  " << nownum << " .  Word :  _" << TestRet << "_ ." << endl;
	RetWord = checkChar(TypeIN, TestRet);
	if (RetWord == 1) {
		string OLDWD = GetGlobal(defaultAPI);
		if (OLDWD == "readini-failed") {
		    WriteGlobal(defaultAPI, TestRet);
		}
		else {
			string CURRENTWORD = OLDWD + "." + TestRet;
			WriteGlobal(defaultAPI, CURRENTWORD);
		}	
	}
	nownum++;
	
	EasyIntGlobal("Xs~WordScanAPINum", nownum);
	goto ReScan;
}
void autodict(string Dict) {

	if (_access(Dict.c_str(), 0)) {}
	else {
		remove(Dict.c_str());
	}

	writeini(Dict, "XsDict", "$XsAuth", "Unknown");
	writeini(Dict, "XsDict", "$ID", "New Project");
	writeini(Dict, "XsDict", "$XsNameID", "New Project");

	writeini(Dict, "XsForward", "$maxnum", "0");
	writeini(Dict, "XsAsk", "$starttype", "Hello Im FoxaXu. can i help you?");
	writeini(Dict, "XsAsk", "$Unknown", "I cant understand this mean _typeinfo_. can you tell me what this?");
	writeini(Dict, "XsAsk", "$WordInfo", " Please tell me the Key Word.");
	writeini(Dict, "XsAsk", "$answer", "Please tell me the answer.");
	writeini(Dict, "XsAsk", "$manswer", "Is there more ways to reply to this conversation?");
	writeini(Dict, "XsAsk", "$error", "sorry i cant answer this question.");
}
int checkmaxnum(string Dict, string Part) {
	string checkmnd = readini(Dict, Part, "$maxnum");
	if (checkmnd == "readini-failed") {
		return 0;
	}
	else {
		return 1;
	}
}
void DictEditor(string DictFile) {
	if (_access(DictFile.c_str(), 0)) {
		MessageBox(0, "Error. DictEditor Cannot Open this File.", DictFile.c_str(), 0);
		return;
	}
	string shellcsl;
BACKEDITSHELL:
	system("cls");
	cout << "Dict Editor." << endl;
	cout << "Open File :  " << DictFile << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	string cmds = "type \"" + DictFile + "\"";
	system(cmds.c_str());
	cout << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	cout << endl;
	cout << "np --open notepad    edit --editChar    add --addMRA    exit -- ExitDictEditor" << endl;
	cout << "Edit Shell   _ $ ";
	getline(cin, shellcsl);
	cout << endl;
	if (shellcsl == "np") {
		ShellExecute(0, "open", "notepad", DictFile.c_str(), 0, SW_SHOW);
		goto BACKEDITSHELL;
	}
	if (shellcsl == "edit") {
		string part,chars,datas;
		cout << "Select Part" << endl;
		cout << "SL   _ $ ";
		getline(cin, part);
		cout << endl;
		cout << "select char" << endl;
		cout << "SC   _ $ ";
		getline(cin, chars);
		cout << endl;
		cout << "New Data" << endl;
		cout << "ND   _ $ ";
		getline(cin, datas);

		writeini(DictFile, part, chars, datas);
		goto BACKEDITSHELL;
	}
	if (shellcsl == "add") {
		string spd,newda;
		cout << "select part" << endl;
		cout << "SP   _ $ ";
		getline(cin, spd);

		int retspd = checkmaxnum(DictFile, spd);
		if (retspd == 0) {
			cout << "cannot edit this char,maxnum is not support." << endl;
			system("pause");
			goto BACKEDITSHELL;
		}

		cout << "new data" << endl;
		cout << "Nw   _ $ ";
		getline(cin, newda);

		WriteNewMRA(DictFile, spd, newda);
		goto BACKEDITSHELL;
	}
	if (shellcsl == "exit") {
		return;
	}
}
string ModelTrain(string Dict) {
	system("cls");
	string ImportWordSet, AnswerSet, mksure, newpart,mksr;
	cout << GetAsk(Dict, "$WordInfo") << endl;
	cout << "set word. 使用 exit 取消   _ $";
	getline(cin, ImportWordSet);
	if (ImportWordSet == "exit") {
		RTCode = "cancel";
		return RTCode;
	}
	newpart = ImportWordSet + "-forward";
	int checkWordExist = existMRA(Dict, "XsForward", ImportWordSet);
	if (checkWordExist == 1){
		cout << "Word 已经存在，只进行修改" << endl;
		goto MwriteSet;
	}

	//注册XsForward
	WriteNewMRA(Dict, "XsForward", ImportWordSet);
	//Answer
	BACKANS:
	system("cls");
	cout << GetAsk(Dict, "$answer") << endl;
	cout << "使用exit取消" << endl;
	cout << "Tell   _ $ ";
	getline(cin, AnswerSet);
	system("cls");
	if (AnswerSet == "exit") {
		RTCode = "cancel";
		return RTCode;
	}
	//新注册
		writeini(Dict, newpart, "$maxnum", "1");
		writeini(Dict, newpart, "MRA1", AnswerSet);
		system("cls");
		cout << GetAsk(Dict, "$manswer") << endl;
		cout << " y / n >";
		getline(cin, mksr);
		if (mksr == "n") {
			cout << GetAsk(Dict, "$nanswer") << endl;
			RTCode = "OK";
			return RTCode;
		}
			//多写入
		MwriteSet:
			string moreasw;

		BACKMSR:

			cout << endl;
			cout << GetAsk(Dict, "$nanswer") << endl;
			cout << "Write More answer in here   _ $ ";
			getline(cin, moreasw);
			if (moreasw == "exit") {
				RTCode = "OK";
				return RTCode;
			}
			WriteNewMRA(Dict, newpart, moreasw);
			cout << endl;
			cout << GetAsk(Dict, "$nanswer") << endl;
			cout << "Write OK" << endl;
			goto BACKMSR;
}

int SearchMRA(string Dict, string WordSet,string Block) {
	if (_access(Dict.c_str(), 0)) {
		MessageBox(0, "Dict文件无法打开，请检查输入端是否正常", Dict.c_str(), MB_OK);
		RTCode = "Null Dict file open failed";
		return 0;
	}
	int maxnum= GetMaxNum(Dict, Block);
	int startnums = 1;
	EasyIntGlobal("SearchMRA~s", startnums);

BackScan:
	int localnum = GetIntGlobal("SearchMRA~s");

	if (localnum > maxnum) {
		return 0;
	}

	string lnstr = to_string(localnum);
	string MRASEL = "MRA" + lnstr;
	string getword = readini(Dict, Block, MRASEL);
	if (getword == WordSet) {
		//找到目标
		return 1;
	}
	
	localnum++;
	EasyIntGlobal("SearchMRA~s", localnum);
	goto BackScan;
}

string autogetword(string Dict, string impword,string TypeIN) {
	int retinfo = SearchMRA(Dict, impword, "XsForward");
	string ReturnPack = impword + "-forward";
	if (retinfo == 1) {
		int TestRET = checkmaxnum(Dict, ReturnPack);
		if (TestRET) {
			int maxnumcur = GetMaxNum(Dict, ReturnPack);
			int randomnum = SpawnRandomNum(1, maxnumcur);
			string randomstr = to_string(randomnum);
			string Outdata = "MRA" + randomstr;
			string RTCode = readini(Dict, ReturnPack, Outdata);
			return RTCode;
		}
		else {
			MessageBox(0, "关键性错误，XsCom遇到了无法处理的问题，映射表所在的对应信息不存在，无法完成计算", impword.c_str(), MB_OK);
			RTCode = "Error";
			return RTCode;
		}
	}
	else {
		//重新计算函数
		int maxnum = GetMaxNum(Dict, "XsForward");
		int startnums = 1;
		EasyIntGlobal("ReScanWord~s", startnums);

	ReBackScan:
		int localnum = GetIntGlobal("ReScanWord~s");

		if (localnum > maxnum) {
			return GetAsk(Dict,"$error");
		}

		string lnstr = to_string(localnum);
		string MRASEL = "MRA" + lnstr;
		string getword = readini(Dict,"XsForward", MRASEL);
		int reinis = checkChar(TypeIN, getword);
		if (reinis == 1) {
			//计算新的返回值
			int antiretinfo = SearchMRA(Dict,getword, "XsForward");
			string ReturnPack = getword + "-forward";
			if (antiretinfo == 1) {
				int TestRET = checkmaxnum(Dict, ReturnPack);
				if (TestRET) {
					int maxnumcur = GetMaxNum(Dict, ReturnPack);
					int randomnum = SpawnRandomNum(1, maxnumcur);
					string randomstr = to_string(randomnum);
					string Outdata = "MRA" + randomstr;
					string RTCode = readini(Dict, ReturnPack, Outdata);
					return RTCode;
				}
				else {
					MessageBox(0, "关键性错误，XsCom遇到了无法处理的问题，映射表所在的对应信息不存在，无法完成计算", impword.c_str(), MB_OK);
					RTCode = "Error";
					return RTCode;
				}
			}
		}

		localnum++;
		EasyIntGlobal("ReScanWord~s", localnum);
		goto ReBackScan;
	}
}

string replaceout(string Dict,string outinfo,string TypeIN) {
	string $para_name = readini(Dict, "XsDict", "$XsNameID");
	string outinfo1 = Replace(outinfo, "_name_", $para_name);
	string outinfo2 = Replace(outinfo1, "_typeinfo_", TypeIN);

	return outinfo2;
}

string WordOut(string Dict, string TypeIN) {
	ReTest:
	if (_access(Dict.c_str(), 0)) {
		MessageBox(0, "Dict文件无法打开，请检查输入端是否正常", Dict.c_str(), MB_OK);
		RTCode = "Null Dict file open failed";
		return RTCode;
	}
	string ImportWord = AutoGetWordX(Dict, TypeIN);
	//cout << "Import Word Return :  _" << ImportWord << "_ . " << endl;
	if (ImportWord == "nullword") {
		//训练模式
		string yesno;
		cout << replaceout(Dict, GetAsk(Dict, "$Unknown"),TypeIN) << endl;
		cout << "y/n  _ $ ";
		getline(cin,yesno);
		if (yesno == "y") {
            string backinfo = ModelTrain(Dict);
			if (backinfo == "cancel") {
				return GetAsk(Dict,"$error");
			}
			return "Thanks your help";
		}
		return GetAsk(Dict,"$error");
	}
	string outinf = autogetword(Dict, ImportWord,TypeIN);
	string routinf = replaceout(Dict, outinf, TypeIN);
	return routinf;
 }

string RandSpawnAB123(string mode) {
	string retab;

	if (mode == "num") {
		int numrad = SpawnRandomNum(27, 36);
		if (numrad == 27) retab = "0";
		if (numrad == 28) retab = "1";
		if (numrad == 29) retab = "2";
		if (numrad == 30) retab = "3";
		if (numrad == 31) retab = "4";
		if (numrad == 32) retab = "5";
		if (numrad == 33) retab = "6";
		if (numrad == 34) retab = "7";
		if (numrad == 35) retab = "8";
		if (numrad == 36) retab = "9";
		return retab;
	}

	if (mode == "abc") {
		int numrad = SpawnRandomNum(1, 26);
		if (numrad == 1) retab = "a";
		if (numrad == 2) retab = "b";
		if (numrad == 3) retab = "c";
		if (numrad == 4) retab = "d";
		if (numrad == 5) retab = "e";
		if (numrad == 6) retab = "f";
		if (numrad == 7) retab = "g";
		if (numrad == 8) retab = "h";
		if (numrad == 9) retab = "i";
		if (numrad == 10) retab = "g";
		if (numrad == 11) retab = "k";
		if (numrad == 12) retab = "l";
		if (numrad == 13) retab = "m";
		if (numrad == 14) retab = "n";
		if (numrad == 15) retab = "o";
		if (numrad == 16) retab = "p";
		if (numrad == 17) retab = "q";
		if (numrad == 18) retab = "r";
		if (numrad == 19) retab = "s";
		if (numrad == 20) retab = "t";
		if (numrad == 21) retab = "u";
		if (numrad == 22) retab = "v";
		if (numrad == 23) retab = "w";
		if (numrad == 24) retab = "x";
		if (numrad == 25) retab = "y";
		if (numrad == 26) retab = "z";
		return retab;
	}

	//All
	int numrad = SpawnRandomNum(1, 36);
	if (numrad == 1) retab = "a";
	if (numrad == 2) retab = "b";
	if (numrad == 3) retab = "c";
	if (numrad == 4) retab = "d";
	if (numrad == 5) retab = "e";
	if (numrad == 6) retab = "f";
	if (numrad == 7) retab = "g";
	if (numrad == 8) retab = "h";
	if (numrad == 9) retab = "i";
	if (numrad == 10) retab = "g";
	if (numrad == 11) retab = "k";
	if (numrad == 12) retab = "l";
	if (numrad == 13) retab = "m";
	if (numrad == 14) retab = "n";
	if (numrad == 15) retab = "o";
	if (numrad == 16) retab = "p";
	if (numrad == 17) retab = "q";
	if (numrad == 18) retab = "r";
	if (numrad == 19) retab = "s";
	if (numrad == 20) retab = "t";
	if (numrad == 21) retab = "u";
	if (numrad == 22) retab = "v";
	if (numrad == 23) retab = "w";
	if (numrad == 24) retab = "x";
	if (numrad == 25) retab = "y";
	if (numrad == 26) retab = "z";
	if (numrad == 27) retab = "0";
	if (numrad == 28) retab = "1";
	if (numrad == 29) retab = "2";
	if (numrad == 30) retab = "3";
	if (numrad == 31) retab = "4";
	if (numrad == 32) retab = "5";
	if (numrad == 33) retab = "6";
	if (numrad == 34) retab = "7";
	if (numrad == 35) retab = "8";
	if (numrad == 36) retab = "9";

	return retab;
}

string selfIDE(string Dict) {
	//自带编辑器
	ReloadIDE:
	if (_access(Dict.c_str(), 0)) {
		MessageBox(0, "Dict文件无法打开，请检查输入端是否正常", Dict.c_str(), MB_OK);
		RTCode = "Null Dict file open failed";
		return RTCode;
	}
	system("cls");
	string StartType = readini(Dict, "XsAsk", "$starttype");
	string XsNameID = readini(Dict, "XsDict", "$XsNameID");
	cout << "Xs Self IDE Mode. Model Dialog Start, Use Exit to Close this Model Dialog" << endl;
	cout << "This API is not good for release" << endl;
	cout << "use train to start Model training. Use reload to back UI" << endl;
	cout << "use edit to open edit XsDict" << endl;
	cout << "Model is Load From :   " << Dict << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << endl;
	cout << XsNameID << " :   " << StartType << endl;
	string LineType;
	string ForwardS;
SBackLine:
	cout << endl;
	cout << "You :  ";
	getline(cin, LineType);
	cout << endl;
	if (LineType == "exit") {
		return RTCode;
	}
	if (LineType == "train") {
		ModelTrain(Dict);
		goto SBackLine;
	}
	if (LineType == "edit") {
		DictEditor(Dict);
		goto ReloadIDE;
	}
	if (LineType == "reload") {
		goto ReloadIDE;
	}
	ForwardS = WordOut(Dict, LineType);
	cout << XsNameID << " :   " << ForwardS << endl;
	goto SBackLine;
}

string BATransLow(string Dict) {

}