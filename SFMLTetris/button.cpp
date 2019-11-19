#include "button.h"
#include<iostream>
#include"SFMLTetris.h"


button::button(float x, float y, float width, float height,
	sf::Font font,std::string msg, unsigned int character_size,
	sf::Color idle_color, sf::Color pressed_color,
	sf::Color textidle_color, sf::Color textpressed_color,
	short unsigned id)
{  
	isClick = false;

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->btstate = idle;
	this->id = id;

	this->textidlecolor = textidle_color;
	this->textpressedcolor = textpressed_color;

	this->idlecolor = idle_color;
	this->pressedcolor = pressed_color;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_color);

	this->font = font;
	this->text.setFont(this->font);
	this->text.setCharacterSize(character_size);
	this->text.setFillColor(textidle_color);
	this->text.setStyle(Text::Bold);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f - 10,
		this->shape.getPosition().y+ this->text.getGlobalBounds().height / 2.f + 10
	);
	this->text.setString(msg);

}

button::~button()
{
}

const bool button::is_pressed() const
{
	if (this->btstate == ispressed)
		return true;

	return false;
}

const std::string button::Gettxt() const
{
	return this->text.getString();
}

const short unsigned button::Getid() const
{
	return this->id;
}

void button::Settxt(const std::string text)
{
	this->text.setString(text);
}

void button::Setid(const short unsigned id)
{
	this->id = id;
}
void button::update(const sf::Vector2i& mousepos,sf::Event event)
{
	this->btstate = idle;
	//按钮被按下
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && mousepos.x >= x && mousepos.x <= x + width && mousepos.y >= y && mousepos.y <= y + height)
	{
		this->btstate = ispressed;
	}
	//按钮松开，说明完成一次点击
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mousepos.x >= x && mousepos.x <= x + width && mousepos.y >= y && mousepos.y <= y + height)
	{
		this->btstate = idle;
		isClick = true;
	}

	switch (this->btstate)
	{
	case idle:
		this->shape.setFillColor(this->idlecolor);
		this->text.setFillColor(this->textidlecolor);
		break;
	case ispressed:
		this->shape.setFillColor(this->pressedcolor);
		this->text.setFillColor(this->textpressedcolor);
		break;
	default:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
		break;
	}
}
void button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}
