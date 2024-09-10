#include "Slide2.h"

// Constructor
Slide2::Slide2() {
	// Init pos, padding
	this->posX = SCREEN_WIDTH / 2.f;
	this->posY = SCREEN_HEIGHT / 2.f;
	this->padding = 10.f;

	// Init button
	this->header = nullptr;
	this->buttonNameComputer = nullptr;
	this->buttonIp = nullptr;
	this->buttonPort = nullptr;
	this->buttonReturn = nullptr;

}

Slide2::~Slide2() {
	delete this->header;
	delete this->buttonNameComputer;
	delete this->buttonIp;
	delete this->buttonPort;
	delete this->buttonReturn;
}


// Set, Get method of PosX
void Slide2::setPosX(float posX) {
	this->posX = posX;
}

float Slide2::getPosX() {
	return this->posX;
}

// Set, Get method of PosY
void Slide2::setPosY(float posY) {
	this->posY = posY;
}

float Slide2::getPosY() {
	return this->posY;
}

// Set, Get method of Padding
void Slide2::setPadding(float padding) {
	this->padding = padding;
}

float Slide2::getPaddding() {
	return this->padding;
}

// Set, Get method of Header
void Slide2::setHeader(Button* header) {
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

Button* Slide2::getHeader() {
	return this->header;
}

// Set, Get method of buttonNameComputer
void Slide2::setButtonNameComputer(Button* buttonNameComputer) {
	this->buttonNameComputer = buttonNameComputer;

	// Set button server position
	setPosX(SCREEN_WIDTH / 2.f);
	setPosY(this->header->getRect().getOrigin().y + this->header->getRect().getSize().y + this->padding);
	this->buttonNameComputer->setPosition(this->posX, this->posY);

	// Set button server color rectangle
	this->buttonNameComputer->setColorRect(serverOff_cRect);

	// Set button server color text
	this->buttonNameComputer->setColorText(serverOn_cText);
	this->buttonNameComputer->setPosText(this->buttonNameComputer->getText().getPosition().x, this->buttonNameComputer->getText().getPosition().y + 5.f);
}

Button* Slide2::getButtonNameComputer() {
	return this->buttonNameComputer;
}

// Set, Get method of buttonIp;
void Slide2::setButtonIp(Button* buttonIp) {
	this->buttonIp = buttonIp;

	// Set button Ip position
	setPadding(padding_items);
	setPosX(SCREEN_WIDTH / 2.f);
	setPosY(this->buttonNameComputer->getRect().getPosition().y + 1 * (this->buttonNameComputer->getRect().getSize().y + this->padding));
	this->buttonIp->setPosition(this->posX, this->posY);

	// Set button Ip rectangle
	this->buttonIp->setColorRect(infoServer_cRect);

	// Set button Ip text
	this->buttonIp->setColorText(infoServer_cText);
	this->buttonIp->setString(this->infoServer.first);
}

Button* Slide2::getButtonIp() {
	return this->buttonIp;
}

// Set, Get method of buttonPort
void Slide2::setButtonPort(Button* buttonPort) {
	this->buttonPort = buttonPort;

	// Set button Port position
	setPadding(padding_items);
	setPosX(SCREEN_WIDTH / 2.f);
	setPosY(this->buttonNameComputer->getRect().getPosition().y + 2 * (this->buttonNameComputer->getRect().getSize().y + this->padding));
	this->buttonPort->setPosition(this->posX, this->posY);

	// Set button Port rectangle
	this->buttonPort->setColorRect(infoServer_cRect);

	// Set button Port text
	this->buttonPort->setColorText(infoServer_cText);
	this->buttonPort->setString(std::to_string(this->infoServer.second));
}

Button* Slide2::getButtonPort() {
	return this->buttonPort;
}

// Set, Get method of buttonReturn
void Slide2::setButtonReturn(Button* buttonReturn) {
	this->buttonReturn = buttonReturn;

	// Set button connected position
	setPosX(SCREEN_WIDTH - this->buttonReturn->getRect().getSize().x);
	setPosY(SCREEN_HEIGHT - this->buttonReturn->getRect().getSize().y - 10.f);
	this->buttonReturn->setPosition(this->posX, this->posY);

	// Set button connected rectangle
	this->buttonReturn->setColorRect(return_cRect);

	// Set button connected text
	this->buttonReturn->setString(return_text);
	this->buttonReturn->setColorText(return_cText);
}

Button* Slide2::getButtonReturn() {
	return this->buttonReturn;
}

// Set, Get method of information server
void Slide2::setInfoServer(std::pair<std::string, int> infoServer) {
	this->infoServer = infoServer;
}

std::pair<std::string, int> Slide2::getInfoServer() {
	return this->infoServer;
}

// Set, Get method of text ip + port
void Slide2::setTextIp(sf::Text textIp) {
	this->textIp = textIp;


	// Set text ip position
	setPosX(this->buttonIp->getText().getPosition().x - distance);
	setPosY(this->buttonIp->getText().getPosition().y - 20.f);
	this->textIp.setPosition(this->posX, this->posY);

	// Set text ip string
	this->textIp.setString(text_ip);
}

sf::Text Slide2::getTextIp() {
	return this->textIp;
}

void Slide2::setTextPort(sf::Text textPort) {
	this->textPort = textPort;

	// Set text port position
	setPosX(this->buttonPort->getText().getPosition().x - distance);
	setPosY(this->buttonPort->getText().getPosition().y - 20.f);
	this->textPort.setPosition(this->posX, this->posY);

	// Set text port string
	this->textPort.setString(text_port);
}

sf::Text Slide2::getTextPort() {
	return this->textPort;
}

// Run
void Slide2::run(sf::RenderWindow& window, Client client) {

	client.run();

	sf::Font fontStandard, fontBold, fontItalic, fontItalicBold;
	if (!fontStandard.loadFromFile(fontStandardPath) || !fontBold.loadFromFile(fontBoldPath) || !fontItalic.loadFromFile(fontItalicPath) || !fontItalicBold.loadFromFile(fontItalicBoldPath)) {
		std::cout << "Fail to load font";
	}

	std::string ip;
	int port;
	ip.assign(client.getConnectPars()[client.getPcIndex()].first);
	port = client.getConnectPars()[client.getPcIndex()].second;

	sf::Text initText;
	initText.setFont(fontBold);
	initText.setFillColor(text_color);

	// Set information server
	setInfoServer({ ip, port });

	// Set button
	setHeader(new Button(button_width, button_height, cRect, text_init, fontItalicBold, cText));
	setButtonNameComputer(new Button(button_width, button_height, cRect, text_init, fontBold, cText));
	setButtonIp(new Button(button_width, button_height, cRect, text_init, fontStandard, cText));
	setButtonPort(new Button(button_width, button_height, cRect, text_init, fontStandard, cText));
	setButtonReturn(new Button(return_width, button_height, cRect, text_init, fontStandard, cText));

	// Set text server
	setTextIp(initText);
	setTextPort(initText);

	// Get name computer
	sockaddr_in hint = client.getServerAddr();
	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on
	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	if (getnameinfo((sockaddr*)&hint, sizeof(hint), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
	}
	else {
		inet_ntop(AF_INET, &hint.sin_addr, host, NI_MAXHOST);
	}
	char ipBuffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(hint.sin_addr), ipBuffer, INET_ADDRSTRLEN);

	std::string nameServer(host);

	this->buttonNameComputer->setString(nameServer);


	while (window.isOpen()) {

		window.clear(sf::Color(180, 180, 179)); // background

		this->header->draw(window);

		this->buttonNameComputer->draw(window);

		this->buttonIp->draw(window);


		this->buttonPort->draw(window);

		this->buttonReturn->draw(window);

		window.draw(this->textIp);

		window.draw(this->textPort);

		sf::Event e;
		while (window.pollEvent(e)) {
			Event event;
			event.setEvent(e);

			if (event.Closed()) window.close();
			if (event.Clicked(this->buttonReturn)) {
				Slide1 slide1;
				client.end();
				slide1.run(window, client);
			}
		}

		window.display();
	}
}