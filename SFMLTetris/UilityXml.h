#pragma once
#include<string>
#include<iostream>
#include <windows.h>
#include "tinyxml.h"
#include"AnswerModule.h"

using namespace std;
class UilityXml
{
public:
	UilityXml();
	~UilityXml();
	static WQuestion * LoadXmlFile(const char* path);	//读取对应的xml文本文件
	
private:
	static string Utf8ToUnicode(const char * szU8);	//将utf-8编码转换成Unicode，解决中文字符乱码
	static string Wchar_tToString(wchar_t *wchar);
	static wstring Utf8ToUnicodeW(const char * szU8);
};

