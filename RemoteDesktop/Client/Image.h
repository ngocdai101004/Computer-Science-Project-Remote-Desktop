#pragma once
#include "main.h"

class Image {
public:
	Image();

	void receiveImages(SOCKET clientSocket);
	void threadImage(SOCKET clientSocket);

};