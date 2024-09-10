#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <cmath>
#include <Windows.h>

const int PORT = 8080;

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

};

Mouse::Mouse() {
    this->coordinate = { 300,150 };
    this->event = MouseEvent::None;
}

Mouse::Mouse(POINT coordinate, MouseEvent event) {
    this->coordinate = coordinate;
    this->event = event;
}

void Mouse::setPosittion(POINT coordinate) {
    this->coordinate = coordinate;
}

POINT Mouse::getPosittion() {
    return this->coordinate;
}

void Mouse::setEvent(MouseEvent event) {
    this->event = event;
}

MouseEvent Mouse::getEvent() {
    return this->event;
}

Mouse mouseState;

// Init server
void initServer(SOCKET& serverSocket) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create server socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to bind" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
    }


    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cout << "Failed to listen" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
    }
}

// Realse socket
void releaseSocket(SOCKET& socket) {
    closesocket(socket);
    WSACleanup();
}

// Image
// Get screen of controlled machine
cv::Mat CaptureScreen() {
    HDC hScreen = GetDC(NULL);
    int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int screenLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screenTop = GetSystemMetrics(SM_YVIRTUALSCREEN);

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    HDC hDC = CreateCompatibleDC(hScreen);
    SelectObject(hDC, hBitmap);
    BitBlt(hDC, 0, 0, screenWidth, screenHeight, hScreen, screenLeft, screenTop, SRCCOPY);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screenWidth;
    bi.biHeight = -screenHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biSizeImage = 0;
    bi.biCompression = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    cv::Mat frame(screenHeight, screenWidth, CV_8UC4); // RGBA
    GetDIBits(hDC, hBitmap, 0, screenHeight, frame.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);

    DeleteObject(hBitmap);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);

    return frame;
}

// Divide data and send to client
void SendImageData(SOCKET clientSocket) {
    const int packetSize = 256;
    auto start_time = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    std::cout << "Here SendCompressedData \n";
    while (1) {

        cv::Mat frame = CaptureScreen();

        cv::resize(frame, frame, cv::Size(), 0.75, 0.75);

        std::vector<uchar> buffer;

        cv::imencode(".jpg", frame, buffer);

        int size = static_cast<int>(buffer.size());

        //std::cout << size << std::endl;

        int sentBytes = 0;
        while (sentBytes < size) {
            int remainingBytes = size - sentBytes;
            int bytesToSend = std::min(remainingBytes, packetSize);

            send(clientSocket, reinterpret_cast<char*>(&bytesToSend), sizeof(int), 0);
            send(clientSocket, reinterpret_cast<char*>(buffer.data()), bytesToSend, 0);

            sentBytes += bytesToSend;

            // Clear
            //std::vector<uchar>::iterator it = buffer.begin() + bytesToSend;
            //std::vector<uchar> temp(buffer.begin(), it);
            buffer = std::vector<uchar>(buffer.begin() + bytesToSend, buffer.end());
            //temp.clear();
        }

        int endMarker = -1;
        send(clientSocket, reinterpret_cast<char*>(&endMarker), sizeof(int), 0);

        frame.release();
        buffer.clear();
        buffer.shrink_to_fit();

        frame_count++;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        if (elapsed_time >= 1000) {
            double fps = frame_count / (elapsed_time / 1000.0);
            std::cout << "FPS: " << fps << std::endl;
            frame_count = 0;
            start_time = std::chrono::high_resolution_clock::now();
        }

        Sleep(1);

    }
}   


// Mouse
// Receive event mouse from client
void ReceiveMouseEvent(SOCKET clientSocket) {
    // While loop: accept and echo message back to client
    char buf[sizeof(int) * 3];
    while (true) {
        memset(buf, 0, sizeof(int) * 3);
        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, sizeof(buf), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in receive mouse events. Quitting" << std::endl;
            mouseState.setPosittion({ -1,-1 });
            mouseState.setEvent(MouseEvent::None);
            break;
        }

        if (bytesReceived == 0) {
            std::cout << "Client disconnected " << std::endl;
            mouseState.setPosittion({ -1,-1 });
            mouseState.setEvent(MouseEvent::None);
            break;
        }

        MouseEvent mouseEvent = MouseEvent::None;
        int x = 0, y = 0, eventData = 0;
        memcpy(&x, buf, sizeof(int));
        memcpy(&y, buf + sizeof(int), sizeof(int));
        memcpy(&eventData, buf + 2 * sizeof(int), sizeof(int));
        // Convert received string to the corresponding mouse event
        if (eventData == 11) {
            mouseEvent = MouseEvent::LeftButtonDown;
        }
        else if (eventData == 10) {
            mouseEvent = MouseEvent::LeftButtonUp;
        }
        else if (eventData == 22) {
            mouseEvent = MouseEvent::LeftDoubleClick;
        }
        else if (eventData == 21) {
            mouseEvent = MouseEvent::RightButtonDown;
        }
        else if (eventData == 20) {
            mouseEvent = MouseEvent::RightButtonUp;
        }
        else if (eventData == 21) {
            mouseEvent = MouseEvent::RightDoubleClick;
        }
        else if (eventData == 31) {
            mouseEvent = MouseEvent::MiddleButtonDown;
        }
        else if (eventData == 30) {
            mouseEvent = MouseEvent::MiddleButtonUp;
        }
        else if (eventData == 40) {
            mouseEvent = MouseEvent::ScrollUp;
        }
        else if (eventData == 41) {
            mouseEvent = MouseEvent::ScrollDown;
        }

        mouseState.setPosittion({ x,y });
        mouseState.setEvent(mouseEvent);
    }
}

// Show event mouse in controlled machine
void putMouse() {
    // Simulate the mouse event
    int formerX = -1;
    int formerY = -1;
    MouseEvent formerEvent = MouseEvent::None;
    while (1) {
        int x = mouseState.getPosittion().x;
        int y = mouseState.getPosittion().y;
        MouseEvent mouseEvent = mouseState.getEvent();
        //Check for changing
        if (x != formerX || y != formerY || formerEvent != mouseEvent) {
            // when client disconnects or error in receive mouse events
            if (x == -1 || y == -1) return;
            formerX = x;
            formerY = y;
            formerEvent = mouseEvent;

            INPUT input{};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
            input.mi.dx = x / 0.75;
            input.mi.dy = y / 0.75;

            switch (mouseEvent) {
            case MouseEvent::LeftButtonDown:
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                break;
            case MouseEvent::LeftButtonUp:
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                break;
            case MouseEvent::RightButtonDown:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                break;
            case MouseEvent::RightButtonUp:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                break;
            case MouseEvent::MiddleButtonDown:
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                break;
            case MouseEvent::MiddleButtonUp:
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
                break;
            case MouseEvent::ScrollUp:
                input.mi.dwFlags = MOUSEEVENTF_WHEEL;
                input.mi.mouseData = WHEEL_DELTA;
                break;
            case MouseEvent::ScrollDown:
                input.mi.dwFlags = MOUSEEVENTF_WHEEL;
                input.mi.mouseData = -WHEEL_DELTA;
                break;
            default:
                input.mi.dwFlags = MOUSEEVENTF_MOVE;
                break;
            }
            SendInput(1, &input, sizeof(INPUT));
            SetCursorPos(input.mi.dx, input.mi.dy);

            POINT cursor;
            GetCursorPos(&cursor);
        }
        Sleep(1);
    }

}

// Keyboard
// Receive data from client and show event keyboard in controlled machine
void ReceiveAndSendKey(SOCKET clientSocket)
{
    bool currentKeys[256] = { false };
    static bool previousKeys[256] = { false };
    memset(currentKeys, 0, sizeof(currentKeys));
    memset(previousKeys, 0, sizeof(previousKeys));
    while (true)
    {
        int key;
        int k;
        int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&k), sizeof(k), 0);
        int bytesRead = recv(clientSocket, reinterpret_cast<char*>(&key), sizeof(key), 0);
        currentKeys[key] = k;
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Failed to receive data" << std::endl;
            break;
        }
        if (bytesRead == SOCKET_ERROR)
        {
            std::cerr << "Failed to receive data." << std::endl;
            break;
        }

        // Show event keyboard
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.wScan = MapVirtualKeyEx(key, MAPVK_VK_TO_VSC, GetKeyboardLayout(0));
        input.ki.dwFlags = 0;

        for (int i = 1; i <= 255; i++) {
            if (currentKeys[i] && !previousKeys[i]) {
                SendInput(1, &input, sizeof(INPUT));
                previousKeys[i] = true;
            }
            else if (!currentKeys[i] && previousKeys[i]) {
                input.ki.dwFlags |= KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
                previousKeys[i] = false;
            }
        }
    }
}

int main() {
    SOCKET serverSocket;
    initServer(serverSocket);

    SOCKADDR_IN clientAddr;

    std::cout << "Hello Client. I am Server" << std::endl;

    int clientAddrSize = sizeof(clientAddr);

    // Accept client
    SOCKET screenClientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    SOCKET mouseClientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    SOCKET keyboardClientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

    // Mouse thread
    if (mouseClientSocket == INVALID_SOCKET)
        std::cerr << "Failed to accept client connection" << std::endl;
    else {
        std::thread receiveMouseEventThread(ReceiveMouseEvent, mouseClientSocket);
        receiveMouseEventThread.detach();
    }

    std::thread putMouseThread(putMouse);
    putMouseThread.detach();

    // Keyboard thread
    if (keyboardClientSocket == INVALID_SOCKET)
        std::cerr << "Failed to accept client connection" << std::endl;
    else {
        std::thread receiveKeyboardEventThread(ReceiveAndSendKey, keyboardClientSocket);
        receiveKeyboardEventThread.detach();
    }

    // Image thread
    if (screenClientSocket == INVALID_SOCKET)
        std::cerr << "Failed to accept client connection" << std::endl;
    else {
        std::thread sendImageThread(SendImageData, screenClientSocket);
        sendImageThread.join();
    }


    releaseSocket(screenClientSocket);
    releaseSocket(mouseClientSocket);
    releaseSocket(keyboardClientSocket);
    releaseSocket(serverSocket);
    cv::destroyAllWindows();

    return 0;
}