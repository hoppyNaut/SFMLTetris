#include "Animation.h"


Animation::Animation(sf::Vector2i _spriteSize, std::string texturePath, int _frameCount, float _animationDuration)
{
	spriteSize = _spriteSize;
	sf::Texture texture;
	if (!texture.loadFromFile(texturePath))
	{
		std::cout << "Í¼Æ¬" + texturePath + "Ã»ÓÐÕÒµ½" << std::endl;
	}

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, spriteSize.x, spriteSize.y));
	sprite.setScale(SCALE, SCALE);
	frameCount = _frameCount;
	animationDuration = _animationDuration;
}

Animation::Animation(sf::Vector2i _spriteSize, sf::Sprite _sprite, int _frameCount, float _animationDuration)
{
	spriteSize = _spriteSize;
	sprite = _sprite;
	frameCount = _frameCount;
	animationDuration = _animationDuration;
}

Animation::~Animation()
{
}

void Animation::Reset()
{
	deltaTime = clock.getElapsedTime();
	targetTime = deltaTime.asSeconds() + animationDuration;
	elapsedTime = deltaTime.asSeconds();

}

bool Animation::PlayAnim(int x,int y,sf::RenderWindow & renderWindow)
{
	if (elapsedTime < targetTime)
	{
		//std::cout << "tagetTime£º" << targetTime << std::endl;
		//std::cout << "elapsedTime£º" << elapsedTime << std::endl;
		int animFrame = static_cast<int>((1 - (targetTime - elapsedTime) / animationDuration) * frameCount) % frameCount;
		//std::cout << animFrame << std::endl;
		sprite.setTextureRect(sf::IntRect(animFrame * spriteSize.x, 0, spriteSize.x, spriteSize.y));
		sprite.setScale(SCALE, SCALE);
		sprite.setPosition(x, y);
		renderWindow.draw(sprite);
		deltaTime = clock.getElapsedTime();
		elapsedTime = deltaTime.asSeconds();
		return true;
	}
	return false;
}
