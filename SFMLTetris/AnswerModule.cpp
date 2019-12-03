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
		cout << "Failed to load file��No root element" << endl;
		//�����ڴ�
		doc.Clear();
		return false;
	}

	//�����ӽڵ�
	for (TiXmlElement * elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		WQuestion wquestion;
		//��ȡԪ����
		string elemName = elem->Value();
		string attr;
		//��ȡԪ������ֵ
		attr = elem->Attribute("ID");
		int index = atoi(attr.c_str());
		//�ٴα��������Խ��
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
		//Ԥת�����õ�����ռ�Ĵ�С;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//ת��
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//������'\0'
	wszString[wcsLen] = '\0';

	string unicodeString = Wchar_tToString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

string AnswerModule::Wchar_tToString(wchar_t * wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char *psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
	string szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
	return szDst;
}

wstring AnswerModule::Utf8ToUnicodeW(const char * szU8)
{
	//Ԥת�����õ�����ռ�Ĵ�С;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//ת��
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//������'\0'
	wszString[wcsLen] = '\0';

	wstring unicodeString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

