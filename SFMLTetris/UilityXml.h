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
	static WQuestion * LoadXmlFile(const char* path);	//��ȡ��Ӧ��xml�ı��ļ�
	
private:
	static string Utf8ToUnicode(const char * szU8);	//��utf-8����ת����Unicode����������ַ�����
	static string Wchar_tToString(wchar_t *wchar);
	static wstring Utf8ToUnicodeW(const char * szU8);
};

