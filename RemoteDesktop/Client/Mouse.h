#ifndef HEADER_FILE
#define HEADER_FILE
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
//#include <Windows.h>
//#include <winuser.h>
#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <queue>
#include <thread>

enum class MouseEvent {
    None,
    LeftButtonDown,
    LeftButtonUp,
    LeftDoubleClick,
    RightButtonDown,
    RightButtonUp,
    RightDoubleClick,
    MiddleButtonDown,
    MiddleButtonUp,
    ScrollUp,
    ScrollDown
};



class Mouse {
private:
    POINT coordinate;
    MouseEvent event;
public:
    Mouse();
    Mouse(POINT coordinate, MouseEvent event);

    void setPosittion(POINT coordinate);
    POINT getPosittion();

    void setEvent(MouseEvent event);
    MouseEvent getEvent();

    void threadMouse(SOCKET clientSocket);

};



bool getBufferMouseInformation(char* buf);
void sendMouseInfomation(SOCKET sock);
void mouseEventControl(int event, int x, int y, int flags, void* userData);

#endif