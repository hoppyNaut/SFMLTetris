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

	Font font;						//字体对象
	Text text;						//文本对象

	SoundBuffer sbDrop, sbAppear;		//音效缓冲
	Sound soundDrop, soundAppear;	//音效
	Music bgMusic, failMusic;	//背景音乐
	int soundVolume;				//背景音乐音量
	bool musicOn;					//背景音乐开关
public:
	Game();
	~Game();
	void GameLoop();
};

#endif // !Game_H
