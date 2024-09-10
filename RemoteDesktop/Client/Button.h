#pragma once

#include "Button.h"
#include "main.h"

class Button {
private:
	sf::RectangleShape rect;
	sf::Text text;
	bool clicked;
public:
	Button();
	Button(float w, float h, const sf::Color& cRect, const std::string& s, const sf::Font& font, const sf::Color& cText);

	// Set, Get method of Rectangle
	void setColorRect(const sf::Color& c);
	void setRect(float w, float h, const sf::Color& c);
	sf::RectangleShape getRect();

	// Set, Get method of Text
	void setString(const std::string& s);
	void setColorText(const sf::Color& c);
	void setPosText(float x, float y);
	void setSizeText(int sizeText);
	void setText(const std::string& s, const sf::Font& font, const sf::Color& c);
	sf::Text getText();

	// Set, Get method of Clicked
	void setClicked(bool clicked);
	bool getClicked();

	// Set method of button
	void setPosition(float posX, float posY);

	// Draw button
	void draw(sf::RenderWindow& window);
};
