#pragma once

#include<string>
#include"tinyxml.h"
#include<iostream>
#include<windows.h>
#include<sstream>
#include<vector>
#include <regex>
using namespace std;

enum AnswerState {
	NoChooseSuject,
	InAnswerQuestion,
	OutOfQuesion,
};

class WQuestion
{
public:
	wstring quesTxt;
	wstring optionA;
	wstring optionB;
	wstring optionC;
	wstring optionD;
	int answer;
};

class AnswerModule
{
public:
	AnswerModule();
	~AnswerModule();

	AnswerState answerState;
	//��������
	WQuestion  wquestionArray[50];
	//��ǰ����
	WQuestion curWQuestion;
	//��ǰ����Ĵ� 1����A��2����B��3����C��4����D
	int curAnswer;
	int questionIndex;

	void Initial();
	void NextQuestion();	//������һ������

	bool LoadXmlFile(const char * path);	//��ȡ��Ӧ��xml�ı��ļ�
private:
	string Utf8ToUnicode(const char * szU8);	//��utf-8����ת����Unicode����������ַ�����
	string Wchar_tToString(wchar_t *wchar);
	wstring Utf8ToUnicodeW(const char * szU8);
};



