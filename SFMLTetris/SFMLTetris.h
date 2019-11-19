#pragma once
#ifndef TETRIS_H
#define TETRIS_H

#include"tinyxml.h"
#include "button.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
#include<string>
#include<vector>
#include <regex>

using namespace std;

#define STAGE_WIDTH 20			//��̨���
#define STAGE_HEIGHT 30			//��̨�߶�
#define WINDOW_WIDTH 80		//���ڿ��
#define WINDOW_HEIGHT 25		//���ڸ߶�
#define GRIDSIZE 25						//����ߴ�
#define SCALE	1.5625						//��������
#define BGSCALEX 0.78125
#define BGSCALEY 0.7324
#define INFO_WIDTH 500			//��Ϣ�����	
#define SpriteTetrisNum 8	
#define SpriteBgNum 2

using namespace sf;

enum eDirection {
	STOP = 0,
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4,
};

enum eColor {
	WHITE = 1,
	RED = 2,
	GREEN = 3,
	BLUE = 4,
	DARKBLUE = 5,
	ORANGE = 6,
	PURPLE = 7,
	YELLOW = 8,
};

class Question {
public:
	string quesTxt;
	string optionA;
	string optionB;
	string optionC;
	string optionD;
	int answer;
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

class SFMLTetris
{
private:
	const int width = STAGE_WIDTH;
	const int height = STAGE_HEIGHT;

	int score;	//�÷�
	int shape;	//��ǰ�������״
	int next_shape;		//��һ���������״

	int head_x;	//	��ǰ�����׸�box��λ��
	int head_y;

	int size_h;		//��ǰ�����size
	int size_w;
	eColor cur_color;		//��ǰ�������ɫ

	int next_size_h;	//��һ�������size
	int next_size_w;
	eColor next_color;	//��һ���������ɫ

	int box_shape[4][4];	//��ǰ������״��4*4����
	int next_box_shape[4][4];	//��һ��������״��4*4����

	int box_map[STAGE_HEIGHT][STAGE_WIDTH];	//��Ϸ���ڵ�box
	int box_Right[4][4];	//�Ҳ���Ϸ��

	bool gameOver;
	bool gameQuit;

	eDirection dir;

	int delay;		//����ˢ����ʱ
	int frameRate;	//֡��
	int timeCounter;		//��ʱ��
	int scoreCount;


	Texture tArray[8];	//���������������
	Sprite spArray[8];	//���龫���������

	string textureTetrisPath[8] = { "Images/Tetris/white.png","Images/Tetris/red.png" ,"Images/Tetris/green.png" ,"Images/Tetris/blue.png" ,
		"Images/Tetris/darkblue.png" ,"Images/Tetris/orange.png" ,"Images/Tetris/purple.png","Images/Tetris/yellow.png" };

	Texture tBackground;
	Sprite spBackground;
	string textureBgPath[2] = { "Images/Bg/BgStar.png","Images/Bg/BgSky.png" };
	int bgIndex = 0;

	Texture tAward,tAwardGray;
	Sprite spAward,spAwardGray;
	string textureAwardPath = "Images/award.png";
	string textureAwardGrayPath = "Images/awardgray.png";

	Font font;						//�������
	Text text;						//�ı�����

	SoundBuffer sbDrop, sbAppear,sbClickCorrect,sbClickFalse;		//��Ч����
	Sound soundDrop, soundAppear,soundClickCorrect,soundClickFalse;	//��Ч
	Music bgMusic, failMusic;	//��������
	int soundVolume;				//������������
	bool musicOn;					//�������ֿ���

	//��ť
	button * btn_optionA;
	button * btn_optionB;
	button * btn_optionC;
	button * btn_optionD;

	//��������
	Question questionArray[50];
	WQuestion wquestionArray[50];
	//��ǰ����
	Question curQuestion;
	WQuestion curWQuestion;
	//��ǰ����Ĵ� 1����A��2����B��3����C��4����D
	int curAnswer;
	int questionIndex;

	//��¼������
	int awardCount;

public:
	SFMLTetris();
	~SFMLTetris();

	void GameLoop();

private:
	int GetRand(int min, int max);//����λ��[min,max]֮��������
	void Initial();							//��ʼ��
	void LoadMediaData();			//�����ز��ļ�
	void Draw();							//����
	void Input();							//��ȡ����
	void ButtonInput(sf::Event event);					//��ť���봦��
	void Logic();							//�߼����� 
	void SetShape(int &cshape, int shape[][4], int &size_w, int &size_h,eColor & color);		//���÷�����״
	void Judge();							//�ж��Ƿ��п���������
	void Rotate();							//��ת����
	bool IsAggin();						//�жϷ������һ���ƶ��Ƿ��Խ����غ�
	bool Exsqr(int row);				//�жϵ�ǰ���Ƿ�Ϊ��
	void Score_Next();					//������һ������
	void ShowNext(int x, int y);		//��ʾ��һ������
	void EliminateRandRow();	//�������һ��
	void EliminateLongestRow();		//����Ԫ������һ��
	void Prompt_info(int x, int y);	//��ʾ��Ϣ
	void ShowGameOverInfo(int x, int y);		//��ʾ��Ϸ������Ϣ
	void ShowSpeedUpInfo(int x, int y);			//��ʾ������ʾ
	void DrawBtn();
	void DrawAward();
	void ControlMusic(Music &music, sf::Event event);	//���Ʊ�������

	bool LoadXmlFile();	//��ȡ��Ӧ��xml�ı��ļ�
	string Utf8ToUnicode(const char * szU8);	//��utf-8����ת����Unicode����������ַ�����
	string Wchar_tToString(wchar_t *wchar);
	wstring Utf8ToUnicodeW(const char * szU8);
	void NextQuestion();	//������һ������
	void ShowQuestion(int x, int y);	//��ʾ����
	void ShowOutOfQuestion(int x, int y);	//��ʾ�������
	std::vector<std::wstring> ws_spilt(const std::wstring & in, const std::wstring & delim);	//wstring�ָ��ַ���
	void JudgeBtnClick(button * btn);
};

#endif // !TETRIS_H





