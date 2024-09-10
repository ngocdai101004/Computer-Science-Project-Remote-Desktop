#pragma once

#include "main.h"
#include "Button.h"

class Event {
private:
	sf::Event event;
public:
	Event();

	// Set, Get method of sf::Event
	void setEvent(sf::Event& event);
	sf::Event getEvent();

	// Event Close
	bool Closed();

	// Event check click button
	bool Clicked(Button* button);
};