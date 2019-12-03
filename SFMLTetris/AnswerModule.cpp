#include "AnswerModule.h"

AnswerModule::AnswerModule()
{
	answerState = NoChooseSuject;
}


AnswerModule::~AnswerModule()
{
}

void AnswerModule::Initial()
{
	questionIndex = 0;
	curWQuestion = wquestionArray[questionIndex];
	curAnswer = curWQuestion.answer;
}

void AnswerModule::NextQuestion()
{
	questionIndex++;
	curWQuestion = wquestionArray[questionIndex];
	curAnswer = curWQuestion.answer;
}

bool AnswerModule::LoadXmlFile(const char * path)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(path))
	{
		cout << doc.ErrorDesc() << endl;
		return false;
	}

	TiXmlElement * root = doc.FirstChildElement();
	if (root == NULL)
	{
		cout << "Failed to load file：No root element" << endl;
		//清理内存
		doc.Clear();
		return false;
	}

	//遍历子节点
	for (TiXmlElement * elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		WQuestion wquestion;
		//获取元素名
		string elemName = elem->Value();
		string attr;
		//获取元素属性值
		attr = elem->Attribute("ID");
		int index = atoi(attr.c_str());
		//再次遍历子属性结点
		for (TiXmlElement *node = elem->FirstChildElement(); node != NULL; node = node->NextSiblingElement())
		{
			string nodeName = node->Value();
			TiXmlNode* node2 = node->FirstChild();
			if (nodeName.compare("question") == 0)
			{
				wquestion.quesTxt = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionA") == 0)
			{
				wquestion.optionA = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionB") == 0)
			{
				wquestion.optionB = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionC") == 0)
			{
				wquestion.optionC = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionD") == 0)
			{
				wquestion.optionD = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("answer") == 0)
			{
				wquestion.answer = atoi(node2->ToText()->Value());
			};
		}
		wquestionArray[index] = wquestion;
	}

	return true;
}

string AnswerModule::Utf8ToUnicode(const char * szU8)
{
		//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	string unicodeString = Wchar_tToString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

string AnswerModule::Wchar_tToString(wchar_t * wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	string szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
	return szDst;
}

wstring AnswerModule::Utf8ToUnicodeW(const char * szU8)
{
	//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	wstring unicodeString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

