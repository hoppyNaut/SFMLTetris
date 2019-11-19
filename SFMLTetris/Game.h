#pragma once
#ifndef Game_H
#define  Game_H

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<string>

using namespace std;
using namespace sf;

class Game
{
private:
	Texture tBackground;
	Sprite spBackground;
	string textureBgPath[2] = { "Images/Bg/BgStar.png","Images/Bg/BgSky.png" };
	int bgIndex = 0;

	Font font;						//�������
	Text text;						//�ı�����

	SoundBuffer sbDrop, sbAppear;		//��Ч����
	Sound soundDrop, soundAppear;	//��Ч
	Music bgMusic, failMusic;	//��������
	int soundVolume;				//������������
	bool musicOn;					//�������ֿ���
public:
	Game();
	~Game();
	void GameLoop();
};

#endif // !Game_H
