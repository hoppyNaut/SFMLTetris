#pragma once
#ifndef TETRIS_H
#define TETRIS_H

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

#define STAGE_WIDTH 20			//��̨���
#define STAGE_HEIGHT 30			//��̨�߶�
#define WINDOW_WIDTH 80		//���ڿ��
#define WINDOW_HEIGHT 25		//���ڸ߶�
#define GRIDSIZE 25						//����ߴ�
#define SCALE	1.5625						//��������
#define INFO_WIDTH 500			//��Ϣ�����	

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

	bool gameOver;
	bool gameQuit;

	eDirection dir;

	int delay;		//����ˢ����ʱ
	int frameRate;	//֡��
	int timeCounter;		//��ʱ��
	int scoreCount;

	Texture tWhite;		//�������
	Sprite spWhite;		//�������
	Texture tGreen;
	Sprite spGreen;
	Texture tRed;
	Sprite spRed;
	Font font;						//�������
	Text text;						//�ı�����

	SoundBuffer sbDrop, sbAppear;		//��Ч����
	Sound soundDrop, soundAppear;	//��Ч
	Music bgMusic, failMusic;	//��������
	int soundVolume;				//������������
	bool musicOn;					//�������ֿ���

public:
	SFMLTetris();
	~SFMLTetris();

	void GameLoop();

private:
	int GetRand(int min, int max);//����λ��[min,max]֮��������
	void Initial();							//��ʼ��
	void Draw();							//����
	void Input();							//��ȡ����
	void Logic();							//�߼����� 
	void SetShape(int &cshape, int shape[][4], int &size_w, int &size_h);		//���÷�����״
	void Judge();							//�ж��Ƿ��п���������
	void Rotate();							//��ת����
	bool IsAggin();						//�жϷ������һ���ƶ��Ƿ��Խ����غ�
	bool Exsqr(int row);				//�жϵ�ǰ���Ƿ�Ϊ��
	void Score_Next();					//������һ������
	void ShowNext(int x, int y);		//��ʾ��һ������
	void Prompt_info(int x, int y);	//��ʾ��Ϣ
	void ShowGameOverInfo(int x, int y);		//��ʾ��Ϸ������Ϣ
	void ShowSpeedUpInfo(int x, int y);			//��ʾ������ʾ
	void ControlMusic(Music &music, sf::Event event);	//���Ʊ�������
};

#endif // !TETRIS_H



