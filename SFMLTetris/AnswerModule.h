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
	//问题数组
	WQuestion  wquestionArray[50];
	//当前问题
	WQuestion curWQuestion;
	//当前问题的答案 1——A；2——B；3——C；4——D
	int curAnswer;
	int questionIndex;

	void Initial();
	void NextQuestion();	//更换下一个问题

	bool LoadXmlFile(const char * path);	//读取对应的xml文本文件
private:
	string Utf8ToUnicode(const char * szU8);	//将utf-8编码转换成Unicode，解决中文字符乱码
	string Wchar_tToString(wchar_t *wchar);
	wstring Utf8ToUnicodeW(const char * szU8);
};



