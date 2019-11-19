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

#define STAGE_WIDTH 20			//舞台宽度
#define STAGE_HEIGHT 30			//舞台高度
#define WINDOW_WIDTH 80		//窗口宽度
#define WINDOW_HEIGHT 25		//窗口高度
#define GRIDSIZE 25						//纹理尺寸
#define SCALE	1.5625						//纹理缩放
#define BGSCALEX 0.78125
#define BGSCALEY 0.7324
#define INFO_WIDTH 500			//信息窗宽度	
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

	int score;	//得分
	int shape;	//当前方块的形状
	int next_shape;		//下一个方块的形状

	int head_x;	//	当前方块首个box的位置
	int head_y;

	int size_h;		//当前方块的size
	int size_w;
	eColor cur_color;		//当前方块的颜色

	int next_size_h;	//下一个方块的size
	int next_size_w;
	eColor next_color;	//下一个方块的颜色

	int box_shape[4][4];	//当前方块形状的4*4数组
	int next_box_shape[4][4];	//下一个方块形状的4*4数组

	int box_map[STAGE_HEIGHT][STAGE_WIDTH];	//游戏框内的box
	int box_Right[4][4];	//右侧游戏框

	bool gameOver;
	bool gameQuit;

	eDirection dir;

	int delay;		//画面刷新延时
	int frameRate;	//帧率
	int timeCounter;		//计时器
	int scoreCount;


	Texture tArray[8];	//方块纹理对象数组
	Sprite spArray[8];	//方块精灵对象数组

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

	Font font;						//字体对象
	Text text;						//文本对象

	SoundBuffer sbDrop, sbAppear,sbClickCorrect,sbClickFalse;		//音效缓冲
	Sound soundDrop, soundAppear,soundClickCorrect,soundClickFalse;	//音效
	Music bgMusic, failMusic;	//背景音乐
	int soundVolume;				//背景音乐音量
	bool musicOn;					//背景音乐开关

	//按钮
	button * btn_optionA;
	button * btn_optionB;
	button * btn_optionC;
	button * btn_optionD;

	//问题数组
	Question questionArray[50];
	WQuestion wquestionArray[50];
	//当前问题
	Question curQuestion;
	WQuestion curWQuestion;
	//当前问题的答案 1——A；2——B；3——C；4——D
	int curAnswer;
	int questionIndex;

	//记录奖励数
	int awardCount;

public:
	SFMLTetris();
	~SFMLTetris();

	void GameLoop();

private:
	int GetRand(int min, int max);//生成位于[min,max]之间的随机数
	void Initial();							//初始化
	void LoadMediaData();			//加载素材文件
	void Draw();							//绘制
	void Input();							//获取输入
	void ButtonInput(sf::Event event);					//按钮输入处理
	void Logic();							//逻辑处理 
	void SetShape(int &cshape, int shape[][4], int &size_w, int &size_h,eColor & color);		//设置方块形状
	void Judge();							//判断是否有可以消的行
	void Rotate();							//旋转方块
	bool IsAggin();						//判断方块的下一次移动是否会越界或重合
	bool Exsqr(int row);				//判断当前行是否为空
	void Score_Next();					//生成下一个方块
	void ShowNext(int x, int y);		//显示下一个方块
	void EliminateRandRow();	//随机消除一行
	void EliminateLongestRow();		//消除元素最多的一行
	void Prompt_info(int x, int y);	//提示信息
	void ShowGameOverInfo(int x, int y);		//显示游戏结束信息
	void ShowSpeedUpInfo(int x, int y);			//显示加速提示
	void DrawBtn();
	void DrawAward();
	void ControlMusic(Music &music, sf::Event event);	//控制背景音乐

	bool LoadXmlFile();	//读取对应的xml文本文件
	string Utf8ToUnicode(const char * szU8);	//将utf-8编码转换成Unicode，解决中文字符乱码
	string Wchar_tToString(wchar_t *wchar);
	wstring Utf8ToUnicodeW(const char * szU8);
	void NextQuestion();	//更换下一个问题
	void ShowQuestion(int x, int y);	//显示问题
	void ShowOutOfQuestion(int x, int y);	//显示问题答完
	std::vector<std::wstring> ws_spilt(const std::wstring & in, const std::wstring & delim);	//wstring分隔字符串
	void JudgeBtnClick(button * btn);
};

#endif // !TETRIS_H





