#pragma once
#ifndef TETRIS_H
#define TETRIS_H

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

#define STAGE_WIDTH 20			//舞台宽度
#define STAGE_HEIGHT 30			//舞台高度
#define WINDOW_WIDTH 80		//窗口宽度
#define WINDOW_HEIGHT 25		//窗口高度
#define GRIDSIZE 25						//纹理尺寸
#define SCALE	1.5625						//纹理缩放
#define INFO_WIDTH 500			//信息窗宽度	

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

	bool gameOver;
	bool gameQuit;

	eDirection dir;

	int delay;		//画面刷新延时
	int frameRate;	//帧率
	int timeCounter;		//计时器
	int scoreCount;

	Texture tWhite;		//纹理对象
	Sprite spWhite;		//精灵对象
	Texture tGreen;
	Sprite spGreen;
	Texture tRed;
	Sprite spRed;
	Font font;						//字体对象
	Text text;						//文本对象

	SoundBuffer sbDrop, sbAppear;		//音效缓冲
	Sound soundDrop, soundAppear;	//音效
	Music bgMusic, failMusic;	//背景音乐
	int soundVolume;				//背景音乐音量
	bool musicOn;					//背景音乐开关

public:
	SFMLTetris();
	~SFMLTetris();

	void GameLoop();

private:
	int GetRand(int min, int max);//生成位于[min,max]之间的随机数
	void Initial();							//初始化
	void Draw();							//绘制
	void Input();							//获取输入
	void Logic();							//逻辑处理 
	void SetShape(int &cshape, int shape[][4], int &size_w, int &size_h);		//设置方块形状
	void Judge();							//判断是否有可以消的行
	void Rotate();							//旋转方块
	bool IsAggin();						//判断方块的下一次移动是否会越界或重合
	bool Exsqr(int row);				//判断当前行是否为空
	void Score_Next();					//生成下一个方块
	void ShowNext(int x, int y);		//显示下一个方块
	void Prompt_info(int x, int y);	//提示信息
	void ShowGameOverInfo(int x, int y);		//显示游戏结束信息
	void ShowSpeedUpInfo(int x, int y);			//显示加速提示
	void ControlMusic(Music &music, sf::Event event);	//控制背景音乐
};

#endif // !TETRIS_H



