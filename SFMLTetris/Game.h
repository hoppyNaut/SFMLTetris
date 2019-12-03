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

#define WINDOW_WIDTH 80		//窗口宽度
#define WINDOW_HEIGHT 25		//窗口高度
#define GRIDSIZE 25						//纹理尺寸
#define SCALE	1.5625						//纹理缩放
#define BGSCALEX 1
#define BGSCALEY 1
#define INFO_WIDTH 500			//信息窗宽度	
#define SpriteTetrisNum 8	
#define SpriteBgNum 2

class Game
{
private:
	sf::RenderWindow window;
	bool gameQuit;

	int delay;		//画面刷新延时
	int frameRate;	//帧率
	int timeCounter;		//计时器
	int scoreCount;


	Texture tBackground;
	Sprite spBackground;
	string textureBgPath[SpriteBgNum] = { "Images/Bg/Bg1.png","Images/Bg/Bg2.png"};
	int bgIndex = 0;

	Texture tAward, tAwardGray,tEliminate;
	Sprite spAward, spAwardGray,spEliminate;
	string textureAwardPath = "Images/award.png";
	string textureAwardGrayPath = "Images/awardgray.png";

	Font font;						//字体对象
	Text text;						//文本对象

	SoundBuffer sbClickCorrect, sbClickFalse,sbClickBtn;		//音效缓冲
	Sound soundClickCorrect, soundClickFalse,soundClickBtn;	//音效
	Music bgMusic, failMusic;	//背景音乐
	int soundVolume;				//背景音乐音量
	bool musicOn;					//背景音乐开关

	//选项按钮
	button * btn_optionA;
	button * btn_optionB;
	button * btn_optionC;
	button * btn_optionD;

	//选择系列题目按钮
	button * btn_Item1;
	button * btn_Item2;
	button * btn_Item3;

	//返回按钮
	button * btn_back;

	AnswerModule answerModule;
	SFMLTetris tetrisModule;

	//记录奖励数
	int awardCount;

	//bool isChooseSubject;

public:
	Game();
	~Game();

	void GameLoop();

private:
	void Initial();		//初始化
	void LoadMediaData();			//加载素材文件

	void Input();		//获取输入
	void ButtonInput(sf::Event event);					//按钮输入处理

	void Logic();

	void Draw();		//绘制
	void DrawAward();
	void Prompt_info(int x, int y);	//绘制提示信息
	void DrawOptionBtn();		//绘制按钮
	void DrawChooseSubjectBtn();
	void DrawBackBtn();
	void ShowQuestion(int x, int y);	//显示问题
	void ShowOutOfQuestion(int x, int y);	//显示问题答完
	void ShowGameOverInfo(int x, int y);		//显示游戏结束信息
	void ShowSpeedUpInfo(int x, int y);			//显示加速提示

	void ControlMusic(Music &music, sf::Event event);	//控制背景音乐
	void JudgeOptionBtnClick(button * btn);
	void JudgeChooseSubjectBtn(button *btn);
	void JudgeBackBtn(button *btn);

	std::vector<std::wstring> ws_spilt(const std::wstring & in, const std::wstring & delim);	//wstring分隔字符串
};

#endif // !Game_H
