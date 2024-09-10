#include "Event.h"

// Constructor
Event::Event() {

}

// Set, Get method of sf::Event
void Event::setEvent(sf::Event& event) {
	this->event = event;
}

sf::Event Event::getEvent() {
	return this->event;
}


// Event Close
bool Event::Closed() {
	return (this->event.type == sf::Event::Closed);
}

// Event check click button
bool Event::Clicked(Button* button) {
	if (this->event.type == sf::Event::MouseButtonPressed) {
		if (button->getRect().getGlobalBounds().contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
			button->setClicked(!button->getClicked());
			return 1;
		}
	}
	return 0;
}