#pragma once

#include "main.h"
#include "Button.h"
#include "Event.h"
#include "Client.h"
#include "Slide2.h"

class Slide1 {
private:
	// Position of objects in graphics
	float posX;
	float posY;
	float padding;

	// Header
	Button* header;

	// Button
	Button* buttonServer;
	std::vector<Button*> buttonItems;
	Button* buttonConnected;

public:
	Slide1();
	~Slide1();

	// Set, Get method of PosX
	void setPosX(float posX);
	float getPosX();

	// Set, Get method of PosY
	void setPosY(float posY);
	float getPosY();

	// Set, Get method of Padding
	void setPadding(float padding);
	float getPaddding();

	// Set, Get method of Header
	void setHeader(Button* header);
	Button* getHeader();

	// Set, Get method of buttonServer
	void setButtonServer(Button* buttonServer);
	Button* getButtonServer();

	// Set, Get method of buttonItems
	void setButtonItems(std::vector<Button*> buttonItems);
	std::vector<Button*> getButtonItems();

	// Set, Get method of buttonConnected
	void setButtonConnected(Button* buttonConnected);
	Button* getButtonConnected();

	// Run
	void run(sf::RenderWindow& window, Client client);
};