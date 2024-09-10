#include "Slide1.h"
#include "Slide2.h"
#include "Client.h"
#include "Mouse.h"
Mouse mouseState;

int main() {


	// Set ip and port
	int numOfServer;
	std::cout << "Enter the number of servers: ";
	std::cin >> numOfServer;

	if (numOfServer > 0) {
		std::vector<std::pair<char*, int>> connected_params(numOfServer);
		for (int i = 0; i < connected_params.size(); i++) {
			char* ip = new char[20];
			int port = 8080;
			std::cout << "Enter IP Adress: ";
			std::cin >> ip;
			std::cin.ignore();
			connected_params[i] = { ip, port };
		}
		sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Network Programming");
		Slide1 slide1;
		Client client;

		client.setConnectPars(connected_params);

		slide1.run(window, client);

		for (int i = 0; i < connected_params.size(); i++) {
			delete connected_params[i].first;
		}
	}

	return 0;
}