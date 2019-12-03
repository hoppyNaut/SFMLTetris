#pragma once
#ifndef Game_H
#define  Game_H

#include"SFMLTetris.h"
#include"AnswerModule.h"
#include"Utility.h"
#include"button.h"
#include "Animation.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;
using namespace sf;

#define WINDOW_WIDTH 80		//���ڿ��
#define WINDOW_HEIGHT 25		//���ڸ߶�
#define GRIDSIZE 25						//����ߴ�
#define SCALE	1.5625						//��������
#define BGSCALEX 1
#define BGSCALEY 1
#define INFO_WIDTH 500			//��Ϣ�����	
#define SpriteTetrisNum 8	
#define SpriteBgNum 2

class Game
{
private:
	sf::RenderWindow window;
	bool gameQuit;

	int delay;		//����ˢ����ʱ
	int frameRate;	//֡��
	int timeCounter;		//��ʱ��
	int scoreCount;


	Texture tBackground;
	Sprite spBackground;
	string textureBgPath[SpriteBgNum] = { "Images/Bg/Bg1.png","Images/Bg/Bg2.png"};
	int bgIndex = 0;

	Texture tAward, tAwardGray,tEliminate;
	Sprite spAward, spAwardGray,spEliminate;
	string textureAwardPath = "Images/award.png";
	string textureAwardGrayPath = "Images/awardgray.png";

	Font font;						//�������
	Text text;						//�ı�����

	SoundBuffer sbClickCorrect, sbClickFalse,sbClickBtn;		//��Ч����
	Sound soundClickCorrect, soundClickFalse,soundClickBtn;	//��Ч
	Music bgMusic, failMusic;	//��������
	int soundVolume;				//������������
	bool musicOn;					//�������ֿ���

	//ѡ�ť
	button * btn_optionA;
	button * btn_optionB;
	button * btn_optionC;
	button * btn_optionD;

	//ѡ��ϵ����Ŀ��ť
	button * btn_Item1;
	button * btn_Item2;
	button * btn_Item3;

	//���ذ�ť
	button * btn_back;

	AnswerModule answerModule;
	SFMLTetris tetrisModule;

	//��¼������
	int awardCount;

	//bool isChooseSubject;

public:
	Game();
	~Game();

	void GameLoop();

private:
	void Initial();		//��ʼ��
	void LoadMediaData();			//�����ز��ļ�

	void Input();		//��ȡ����
	void ButtonInput(sf::Event event);					//��ť���봦��

	void Logic();

	void Draw();		//����
	void DrawAward();
	void Prompt_info(int x, int y);	//������ʾ��Ϣ
	void DrawOptionBtn();		//���ư�ť
	void DrawChooseSubjectBtn();
	void DrawBackBtn();
	void ShowQuestion(int x, int y);	//��ʾ����
	void ShowOutOfQuestion(int x, int y);	//��ʾ�������
	void ShowGameOverInfo(int x, int y);		//��ʾ��Ϸ������Ϣ
	void ShowSpeedUpInfo(int x, int y);			//��ʾ������ʾ

	void ControlMusic(Music &music, sf::Event event);	//���Ʊ�������
	void JudgeOptionBtnClick(button * btn);
	void JudgeChooseSubjectBtn(button *btn);
	void JudgeBackBtn(button *btn);

	std::vector<std::wstring> ws_spilt(const std::wstring & in, const std::wstring & delim);	//wstring�ָ��ַ���
};

#endif // !Game_H
