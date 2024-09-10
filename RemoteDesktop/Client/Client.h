#pragma once

#include "main.h"
#include "Image.h"
#include "Mouse.h"
#include "Keyboard.h"

class Client {
private:
	//Socket for screen, mouse and keyboard transmission
	SOCKET screenClientSocket;
	SOCKET mouseClientSocket;
	SOCKET keyboardClientSocket;
	// Vector about ip and port
	int pcIndex;
	std::vector<std::pair<char*, int>> connectPars;
	SOCKADDR_IN serverAddr;
public:
	//Set, get pcIndex method
	void setPcIndex(int index);
	int getPcIndex();

	// Set, Get method of ip and port
	void setConnectPars(std::vector<std::pair<char*, int>> connectPars);
	std::vector<std::pair<char*, int>> getConnectPars();

	// get server address
	SOCKADDR_IN getServerAddr();

	//init, connect, release sockey
	void initSocket(SOCKET& socket);
	void connectSocketToServer(SOCKET& socket, SOCKADDR_IN& serverAddr, std::vector<std::pair<char*, int>> connectPars, int index);
	void releaseSocket(SOCKET& socket);
	//init, connect, release client
	void initClient();
	void connectClientToServer();
	void releaseClient();

	// run
	void run();

	// Disconnect to server
	void end();

};