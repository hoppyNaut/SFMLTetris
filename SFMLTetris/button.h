#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
enum BT_STATES
{
	idle = 0,
	ispressed
};
class button
{
	
public:
	button(float x, float y, float width, float height,
		   sf::Font font, std::wstring msg, unsigned int character_size,
			int offsetX ,int offsetY,
		   sf::Color idle_color, sf::Color pressed_color,
		   sf::Color textedle_color, sf::Color textpressed_color,
		   short unsigned id);
	~button();
	//property
	bool isClick;
	const bool is_pressed() const;
	const std::string Gettxt() const;
	sf::Text GetText();
	sf::RectangleShape GetShape();
	const short unsigned Getid() const;
	//setting
	void Settxt(const std::string txt);
	void Setid(short unsigned id);
	//render
	void update(const sf::Vector2i& mousepos,sf::Event event);
	void render(sf::RenderTarget& target);
private:
	short unsigned btstate;
	short unsigned id;
	float x, y;
	float width, height;
	sf::RectangleShape shape;
	sf::Font font;
	sf::Text text;

	sf::Color idlecolor;
	sf::Color pressedcolor;

	sf::Color textidlecolor;
	sf::Color textpressedcolor;


};

#endif // !BUTTON_H
