#include "Button.h"

// Constructor
Button::Button() {
	this->clicked = false;
}

Button::Button(float w, float h, const sf::Color& cRect, const std::string& s, const sf::Font& font, const sf::Color& cText) {
	// Attributes default of Rect
	this->rect.setSize(sf::Vector2f(w, h));
	this->rect.setFillColor(cRect);
	this->rect.setOrigin(w / 2, h / 2);

	// Attributes default of Text
	this->text.setString(s);
	this->text.setFont(font);
	this->text.setFillColor(cText);
	this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2);

	// Attributes default of Clicked
	this->clicked = false;
}

// Set Color of Rectangle
void Button::setColorRect(const sf::Color& c) {
	this->rect.setFillColor(c);
}

// Set, Get of Rectangle
void Button::setRect(float w, float h, const sf::Color& c) {
	this->rect.setSize(sf::Vector2f(w, h));
	this->rect.setFillColor(c);
	this->rect.setOrigin(w / 2, h / 2);
}

sf::RectangleShape Button::getRect() {
	return this->rect;
}

// Set string of Text
void Button::setString(const std::string& s) {
	this->text.setString(s);
	this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2 + 10.f);
}

// Set Color of Text
void Button::setColorText(const sf::Color& c) {
	this->text.setFillColor(c);
}

// Set text position
void Button::setPosText(float x, float y) {
	this->text.setPosition(x, y);
}

// Set text size
void Button::setSizeText(int sizeText) {
	this->text.setCharacterSize(sizeText);
}

// Set, Get method of Text
void Button::setText(const std::string& s, const sf::Font& font, const sf::Color& c) {
	this->text.setString(s);
	this->text.setFont(font);
	this->text.setFillColor(c);
	this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2);
}

sf::Text Button::getText() {
	return this->text;
}

// Set, Get method of Clicked
void Button::setClicked(bool clicked) {
	this->clicked = clicked;
}

bool Button::getClicked() {
	return this->clicked;
}

// Set position of button
void Button::setPosition(float posX, float posY) {
	this->rect.setPosition(posX, posY);
	this->text.setPosition(posX, posY);
}


// Draw button
void Button::draw(sf::RenderWindow& window) {
	window.draw(this->rect);
	window.draw(this->text);
}