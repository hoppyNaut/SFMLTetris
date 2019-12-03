#pragma once
#ifndef TETRIS_H
#define TETRIS_H

#define STAGE_WIDTH 20			//��̨���
#define STAGE_HEIGHT 30			//��̨�߶�
#define GRIDSIZE 25						//����ߴ�
#define INFO_WIDTH 500			//��Ϣ�����	
#define SpriteTetrisNum 8	
#define SCALE	1.5625						//��������ss

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

	int rowToEliminate;

	stack<int> eliminateRowStack;

	eDirection dir;

	Texture tArray[8];	//���������������
	Sprite spArray[8];	//���龫���������

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

	void Initial();							//��ʼ��
	void SetDir(eDirection eDir);
	void Logic();							//�߼����� 
	void Draw(sf::RenderWindow & window);
	void ShowNext(int x, int y,sf::RenderWindow & window);		//������һ������
	void EliminateRandRow();	//�������һ��
	void EliminateLongestRow();		//����Ԫ������һ��
private:
	void SetShape(int &cshape, int shape[][4], int &size_w, int &size_h,eColor & color);		//���÷�����״
	void Judge();							//�ж��Ƿ��п���������
	void Rotate();							//��ת����
	bool IsAggin();						//�жϷ������һ���ƶ��Ƿ��Խ����غ�
	bool Exsqr(int row);				//�жϵ�ǰ���Ƿ�Ϊ��
	void Score_Next();					//������һ������
};

#endif // !TETRIS_H





