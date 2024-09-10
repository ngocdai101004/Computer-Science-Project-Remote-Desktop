#pragma once

#include "main.h"

class Keyboard {

public:
    Keyboard();

    void sendKeyboard(SOCKET sock);
    void threadKeyboard(SOCKET sock);
};