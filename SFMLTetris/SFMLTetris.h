#pragma once
#ifndef TETRIS_H
#define TETRIS_H

#define STAGE_WIDTH 20			//舞台宽度
#define STAGE_HEIGHT 30			//舞台高度
#define GRIDSIZE 25						//纹理尺寸
#define INFO_WIDTH 500			//信息窗宽度	
#define SpriteTetrisNum 8	
#define SCALE	1.5625						//纹理缩放ss

#include"Utility.h"
#include "Animation.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<list>
#include<stack>

using namespace std;
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



class SFMLTetris
{
public:
	const int width = STAGE_WIDTH;
	const int height = STAGE_HEIGHT;

	bool gameOver;
	bool isEliminate;

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

	int rowToEliminate;

	stack<int> eliminateRowStack;

	eDirection dir;

	Texture tArray[8];	//方块纹理对象数组
	Sprite spArray[8];	//方块精灵对象数组

	string textureTetrisPath[8] = { "Images/Tetris/white.png","Images/Tetris/red.png" ,"Images/Tetris/green.png" ,"Images/Tetris/blue.png" ,
		"Images/Tetris/darkblue.png" ,"Images/Tetris/orange.png" ,"Images/Tetris/purple.png","Images/Tetris/yellow.png" };

	SoundBuffer sbDrop, sbAppear;
	Sound soundDrop, soundAppear;

	Animation * animEliminate;
	Texture tEliminate;
	Sprite spEliminate;
	string textureEliminatePath = "Images/eliminate.png";
	
public:
	SFMLTetris();
	~SFMLTetris();

	void Initial();							//初始化
	void SetDir(eDirection eDir);
	void Logic();							//逻辑处理 
	void Draw(sf::RenderWindow & window);
	void ShowNext(int x, int y,sf::RenderWindow & window);		//绘制下一个方块
	void EliminateRandRow();	//随机消除一行
	void EliminateLongestRow();		//消除元素最多的一行
private:
	void SetShape(int &cshape, int shape[][4], int &size_w, int &size_h,eColor & color);		//设置方块形状
	void Judge();							//判断是否有可以消的行
	void Rotate();							//旋转方块
	bool IsAggin();						//判断方块的下一次移动是否会越界或重合
	bool Exsqr(int row);				//判断当前行是否为空
	void Score_Next();					//生成下一个方块
};

#endif // !TETRIS_H





