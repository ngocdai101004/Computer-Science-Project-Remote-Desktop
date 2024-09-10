#include "Slide1.h"

// Constructor
Slide1::Slide1() {
	// Init pos, padding
	this->posX = SCREEN_WIDTH / 2.f;
	this->posY = SCREEN_HEIGHT / 2.f;
	this->padding = 10.f;

	// Init button
	this->header = nullptr;
	this->buttonServer = nullptr;
	//this->buttonItems = { nullptr };
	this->buttonConnected = nullptr;

}

Slide1::~Slide1() {
	delete this->header;
	delete this->buttonServer;
	for (int i = 0; i < this->buttonItems.size(); i++) {
		delete this->buttonItems[i];
	}
	delete this->buttonConnected;
}


// Set, Get method of PosX
void Slide1::setPosX(float posX) {
	this->posX = posX;
}

float Slide1::getPosX() {
	return this->posX;
}

// Set, Get method of PosY
void Slide1::setPosY(float posY) {
	this->posY = posY;
}

float Slide1::getPosY() {
	return this->posY;
}

// Set, Get method of Padding
void Slide1::setPadding(float padding) {
	this->padding = padding;
}

float Slide1::getPaddding() {
	return this->padding;
}

// Set, Get method of Header
void Slide1::setHeader(Button* header) {
	this->header = header;

	// Set header rectangle
	this->header->setRect(header_width, header_height, header_cRect);

	// Set header position
	setPosX(SCREEN_WIDTH / 2.f);
	setPosY(this->header->getRect().getOrigin().y);
	this->header->setPosition(this->posX, this->posY);

	// Set header text
	this->header->setString(header_text);
	this->header->setColorText(header_cText);
	this->header->setPosText(200.f, this->header->getRect().getSize().y / 2.f);
}

Button* Slide1::getHeader() {
	return this->header;
}

// Set, Get method of buttonServer
void Slide1::setButtonServer(Button* buttonServer) {
	this->buttonServer = buttonServer;

	// Set button server position
	setPosX(SCREEN_WIDTH / 2.f);
	setPosY(this->header->getRect().getOrigin().y + this->header->getRect().getSize().y + this->padding);
	this->buttonServer->setPosition(this->posX, this->posY);

	// Set button server rectangle
	this->buttonServer->setColorRect(serverOff_cRect);

	// Set button server text
	this->buttonServer->setColorText(serverOff_cText);
	this->buttonServer->setString(server_text);
	this->buttonServer->setPosText(this->buttonServer->getText().getPosition().x, this->buttonServer->getText().getPosition().y + 5.f);
}

Button* Slide1::getButtonServer() {
	return this->buttonServer;
}

// Set, Get method of buttonItems
// Distance between 2 buttonItems equal padding
void Slide1::setButtonItems(std::vector<Button*> buttonItems) {
	this->buttonItems = buttonItems;
	setPadding(padding_items);
	for (int i = 0; i < this->buttonItems.size(); i++) {
		// Set button items position
		setPosX(SCREEN_WIDTH / 2.f);
		setPosY(this->buttonServer->getRect().getPosition().y + (i + 1) * (this->buttonServer->getRect().getSize().y + this->padding));
		this->buttonItems[i]->setPosition(this->posX, this->posY);

		// Set button items rectangle
		this->buttonItems[i]->setColorRect(itemOff_cRect);

		// Set button items text
		/*std::string s = "Computer " + std::to_string(i + 1);
		this->buttonItems[i]->setString(s);*/
		this->buttonItems[i]->setPosText(this->buttonItems[i]->getText().getPosition().x, this->buttonItems[i]->getText().getPosition().y + 5.f);
		this->buttonItems[i]->setColorText(itemOff_cText);
	}
}

std::vector<Button*> Slide1::getButtonItems() {
	return this->buttonItems;
}

// Set, Get method of buttonConnected
void Slide1::setButtonConnected(Button* buttonConnected) {
	this->buttonConnected = buttonConnected;

	// Set button connected position
	setPosX(SCREEN_WIDTH - this->buttonConnected->getRect().getSize().x);
	setPosY(SCREEN_HEIGHT - this->buttonConnected->getRect().getSize().y - 10.f);
	this->buttonConnected->setPosition(this->posX, this->posY);

	// Set button connected rectangle
	this->buttonConnected->setColorRect(connectedOff_cRect);

	// Set button connected text
	this->buttonConnected->setString(connected_text);
	this->buttonConnected->setColorText(connectedOff_cText);
	this->buttonConnected->setPosText(this->buttonConnected->getText().getPosition().x, this->buttonConnected->getText().getPosition().y + 5.f);

}

Button* Slide1::getButtonConnected() {
	return this->buttonConnected;
}

// Run
void Slide1::run(sf::RenderWindow& window, Client client) {
	sf::Font fontStandard, fontBold, fontItalic, fontItalicBold;
	if (!fontStandard.loadFromFile(fontStandardPath) || !fontBold.loadFromFile(fontBoldPath) || !fontItalic.loadFromFile(fontItalicPath) || !fontItalicBold.loadFromFile(fontItalicBoldPath)) {
		std::cout << "Fail to load font";
	}

	std::vector<Button*> buttonTemp; // Vector buttonTemp to set button items
	int selectedItem = -1; // value -1 is item ith isn't chosen, value i is item ith is chosen

	setHeader(new Button(button_width, button_height, cRect, text_init, fontItalicBold, cText));
	setButtonServer(new Button(button_width, button_height, cRect, text_init, fontBold, cText));
	setButtonConnected(new Button(connected_width, button_height, cRect, text_init, fontStandard, cText));

	while (window.isOpen()) {
		window.clear(sf::Color(180, 180, 179)); // background

		this->header->draw(window);

		this->buttonServer->draw(window);

		this->buttonConnected->draw(window);

		sf::Event e;
		while (window.pollEvent(e)) {
			Event event;
			event.setEvent(e);
			if (event.Closed()) window.close();

			// Event click button server
			if (event.Clicked(this->buttonServer)) {
				// If clicked odd times, show the button items
				if (this->buttonServer->getClicked()) {
					// Property of button server on
					this->buttonServer->setColorRect(serverOn_cRect);
					this->buttonServer->setColorText(serverOn_cText);

					// Push item into buttonTemp
					for (int i = 0; i < client.getConnectPars().size(); i++) {
						buttonTemp.push_back(new Button(button_width, button_height, cRect, text_init, fontStandard, cText));
					}
				}
				// If clicked even times, hide the button items
				else {
					// Property of button server off
					this->buttonServer->setColorRect(serverOff_cRect);
					this->buttonServer->setColorText(serverOff_cText);

					// Button connected return default
					this->buttonConnected->setColorRect(connectedOff_cRect);
					this->buttonConnected->setColorText(connectedOff_cText);

					// Clear buttonTemp
					buttonTemp.clear();

					// Return selectedItem to -1
					selectedItem = -1;
				}
				// Set button items
				setButtonItems(buttonTemp);
				for (int i = 0; i < client.getConnectPars().size(); i++) {
					std::string ip;
					ip.assign(client.getConnectPars()[i].first);
					this->buttonItems[i]->setString(ip);
				}
			}


			// Event click button items and set button connected
			for (int i = 0; i < this->buttonItems.size(); i++) {
				if (event.Clicked(this->buttonItems[i])) {
					// If button items ith is chosen
					if (selectedItem == -1) {
						// Update selectedItem
						selectedItem = i;

						// Property of button item ith on
						this->buttonItems[i]->setColorRect(itemOn_cRect);
						this->buttonItems[i]->setColorText(itemOn_cText);

						// Property of connected on
						this->buttonConnected->setColorRect(connectedOn_cRect);
						this->buttonConnected->setColorText(connectedOn_cText);

					}
					// If button items ith isn't chosen
					else if (selectedItem == i) {
						// Update selectedItem
						selectedItem = -1;

						// Property of button item ith off
						this->buttonItems[i]->setColorRect(itemOff_cRect);
						this->buttonItems[i]->setColorText(itemOff_cText);

						// Property of connected off
						this->buttonConnected->setColorRect(connectedOff_cRect);
						this->buttonConnected->setColorText(connectedOff_cText);
					}
				}
			}

			// Event click button connected
			if (selectedItem != -1) {
				// Set index in client connected params
				client.setPcIndex(selectedItem);

				if (event.Clicked(this->buttonConnected)) {
					client.setPcIndex(selectedItem);
					Slide2 slide2;
					slide2.run(window, client);
				}
			}
		}

		// Show button items
		if (this->buttonServer->getClicked()) {
			for (int i = 0; i < this->buttonItems.size(); i++) {
				this->buttonItems[i]->draw(window);
			}
		}

		window.display();
	}

	// Clear buttonTemp
	for (int i = 0; i < buttonTemp.size(); i++) {
		delete buttonTemp[i];
	}

}