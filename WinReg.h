#include"D:\CppHeader\winapicore.h"
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <assert.h>
#include <tchar.h>
#pragma  comment(lib, "shell32.lib")

//BingAI
string ConvertLPCOLESTRToString(LPCOLESTR pOleStr) {
    int stringLength = WideCharToMultiByte(CP_ACP, 0, pOleStr, -1, NULL, 0, NULL, NULL);
    char* pCharStr = new char[stringLength];
    WideCharToMultiByte(CP_ACP, 0, pOleStr, -1, pCharStr, stringLength, NULL, NULL);
    std::string result(pCharStr);
    delete[] pCharStr;
    return result;
}

//BingAI
LPCOLESTR ConvertStringToLPCOLESTR(const std::string& str) {
    int wchars_num = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, wchars_num);
    return wstr;
}

//创建快捷方式
void createlink(string file,string save, string icon, string info) {
    CoInitialize(NULL);
    IShellLink* pShellLink = NULL;
    HRESULT hres;
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);
    if (SUCCEEDED(hres)) {
        pShellLink->SetPath(file.c_str());  // 设置快捷方式目标路径
        pShellLink->SetDescription(info.c_str());  // 设置快捷方式描述
        pShellLink->SetIconLocation(icon.c_str(), 0);  // 设置快捷方式图标

        IPersistFile* pPersistFile;
        hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

        if (SUCCEEDED(hres)) {
            LPCOLESTR saves = ConvertStringToLPCOLESTR(save);

            hres = pPersistFile->Save(saves, TRUE);  // 保存快捷方式到指定位置
            pPersistFile->Release();
        }
        else {
            std::cout << "Failed to create shortcut. Error: " << hres << std::endl;
        }
        pShellLink->Release();
    }
    else {
        std::cout << "Failed to create shortcut. Error: " << hres << std::endl;
    }
    CoUninitialize();
    return;
}

