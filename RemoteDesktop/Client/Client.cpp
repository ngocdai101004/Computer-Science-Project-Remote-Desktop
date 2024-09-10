#include "Client.h"


void Client::setPcIndex(int index) {
	this->pcIndex = index;
}

int Client::getPcIndex() {
	return this->pcIndex;
}
void Client::setConnectPars(std::vector<std::pair<char*, int>> connectPars) {
	this->connectPars = connectPars;
}

std::vector<std::pair<char*, int>> Client::getConnectPars() {
	return this->connectPars;
}

SOCKADDR_IN Client::getServerAddr() {
	return this->serverAddr;
}

void Client::initSocket(SOCKET& sock) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Failed." << std::endl;
		WSACleanup();
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Failed to create client socket." << std::endl;
		WSACleanup();
	}
}

void Client::connectSocketToServer(SOCKET& sock, SOCKADDR_IN& serverAddr, std::vector<std::pair<char*, int>> connectPars, int index) {
	char* ip = connectPars[index].first;
	int port = connectPars[index].second;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) != 1) {
		std::cout << "Failed to convert IP." << std::endl;
		closesocket(sock);
		WSACleanup();
	}

	if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cout << "Failed to connect server." << std::endl;
		closesocket(sock);
		WSACleanup();
	}
}

void Client::releaseSocket(SOCKET& sock) {
	closesocket(sock);
	WSACleanup();
}


void Client::initClient() {
	this->initSocket(this->screenClientSocket);
	this->initSocket(this->mouseClientSocket);
	this->initSocket(this->keyboardClientSocket);
}
void Client::connectClientToServer() {
	this->connectSocketToServer(this->screenClientSocket, this->serverAddr, this->connectPars, this->pcIndex);
	this->connectSocketToServer(this->mouseClientSocket, this->serverAddr, this->connectPars, this->pcIndex);
	this->connectSocketToServer(this->keyboardClientSocket, this->serverAddr, this->connectPars, this->pcIndex);

}
void Client::releaseClient() {
	this->releaseSocket(this->screenClientSocket);
	this->releaseSocket(this->mouseClientSocket);
	this->releaseSocket(this->keyboardClientSocket);
}

void Client::run() {
	this->initClient();
	this->connectClientToServer();

	Mouse mouse;
	mouse.threadMouse(this->mouseClientSocket);

	Keyboard keyboard;
	keyboard.threadKeyboard(this->keyboardClientSocket);

	Image image;
	image.threadImage(this->screenClientSocket);
}

void Client::end() {
	releaseSocket(this->screenClientSocket);
	releaseSocket(this->mouseClientSocket);
	releaseSocket(this->keyboardClientSocket);
	cv::destroyAllWindows();
}