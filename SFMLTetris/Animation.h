#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>

#define SCALE	1.5625	
class Animation
{
public:
	Animation(sf::Vector2i _spriteSize, std::string texturePath,int _frameCount,float _animationDuration);
	Animation(sf::Vector2i _spriteSize, sf::Sprite _sprite, int _frameCount, float _animationDuration);
	~Animation();

	void Reset();
	bool PlayAnim(int x, int y, sf::RenderWindow & renderWindow);

private:
	sf::Vector2i spriteSize;
	sf::Sprite sprite;
	int frameCount;
	float animationDuration ;

	sf::Time deltaTime;
	//sf::Time elapsedTime;
	float elapsedTime = 0;
	float targetTime = 0;
	sf::Clock clock;
};

