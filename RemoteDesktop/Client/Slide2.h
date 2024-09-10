#pragma once

#include "main.h"
#include "Button.h"
#include "Event.h"
#include "Client.h"
#include "Slide1.h"

class Slide2 {
private:
	// Position of objects in graphics
	float posX;
	float posY;
	float padding;

	// Header
	Button* header;

	// Button
	Button* buttonNameComputer;
	Button* buttonIp;
	Button* buttonPort;
	Button* buttonReturn;

	// Ip + port information
	std::pair<std::string, int> infoServer;

	// Text ip + port
	sf::Text textIp;
	sf::Text textPort;

public:
	Slide2();
	~Slide2();

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

	// Set, Get method of buttonNameComputer
	void setButtonNameComputer(Button* buttonNameComputer);
	Button* getButtonNameComputer();

	// Set, Get method of buttonIp;
	void setButtonIp(Button* buttonIp);
	Button* getButtonIp();

	// Set, Get method of buttonPort
	void setButtonPort(Button* buttonPort);
	Button* getButtonPort();

	// Set, Get method of buttonReturn
	void setButtonReturn(Button* buttonReturn);
	Button* getButtonReturn();

	// Set, Get method of information server
	void setInfoServer(std::pair<std::string, int> infoServer);
	std::pair<std::string, int> getInfoServer();

	// Set, Get method of text ip + port
	void setTextIp(sf::Text textIp);
	sf::Text getTextIp();
	void setTextPort(sf::Text textPort);
	sf::Text getTextPort();

	// Run
	void run(sf::RenderWindow& window, Client client);
};