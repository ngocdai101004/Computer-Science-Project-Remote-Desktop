#include "Mouse.h"
#include "main.h"


Mouse::Mouse() {
    this->coordinate = { 0,0 };
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

void Mouse::threadMouse(SOCKET clientSocket) {
    std::thread sendMouseEventThread(sendMouseInfomation, clientSocket);
    sendMouseEventThread.detach();
}

bool getBufferMouseInformation(char* buf) {
    // Convert mouse event to a string representation
    memset(buf, 0, 3 * sizeof(int));
    int eventData = 0;
    switch (mouseState.getEvent()) {
    case MouseEvent::LeftButtonDown:
        eventData = 11;
        break;
    case MouseEvent::LeftButtonUp:
        eventData = 10;
        break;
    case MouseEvent::LeftDoubleClick:
        eventData = 12;
        break;
    case MouseEvent::RightButtonDown:
        eventData = 21;
        break;
    case MouseEvent::RightButtonUp:
        eventData = 20;
        break;
    case MouseEvent::RightDoubleClick:
        eventData = 22;
        break;
    case MouseEvent::MiddleButtonDown:
        eventData = 31;
        break;
    case MouseEvent::MiddleButtonUp:
        eventData = 30;
        break;
    case MouseEvent::ScrollUp:
        eventData = 40;
        break;
    case MouseEvent::ScrollDown:
        eventData = 41;
        break;
    default:
        eventData = 0;
        break;
    }
    int x = mouseState.getPosittion().x, y = mouseState.getPosittion().y;
    memcpy(buf, &x, sizeof(int));
    memcpy(buf + sizeof(int), &y, sizeof(int));
    memcpy(buf + 2 * sizeof(int), &eventData, sizeof(int));
    return true;

}


void sendMouseInfomation(SOCKET sock) {
    int formerX = -1;
    int formerY = -1;
    MouseEvent formerEvent = MouseEvent::None;
    while (1) {
        int x = mouseState.getPosittion().x;
        int y = mouseState.getPosittion().y;
        MouseEvent mouseEvent = mouseState.getEvent();
        //std::cerr << "In before send func (x,y) " << x << " " << y << "\n";
        //std::cerr << "In before send func (former) " << mouseState.getPosittion().x << " " << mouseState.getPosittion().y << "\n";
        if (x != formerX || y != formerY || formerEvent != mouseEvent) {
            formerX = x;
            formerY = y;
            formerEvent = mouseEvent;
            char buf[3 * sizeof(int)];
            bool isVal = getBufferMouseInformation(buf);
            if (isVal)
                send(sock, buf, 3 * sizeof(int), 0);
        }
        Sleep(1);
    }
}

void mouseEventControl(int event, int x, int y, int flags, void* userData) {
    if (event == cv::EVENT_MOUSEMOVE) {
        mouseState.setPosittion({ x,y });
        mouseState.setEvent(MouseEvent::None);
    }
    else if (event == cv::EVENT_LBUTTONDOWN) {
        mouseState.setPosittion({ x, y });
        mouseState.setEvent(MouseEvent::LeftButtonDown);
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        mouseState.setPosittion({ x, y });
        if (mouseState.getEvent() != MouseEvent::LeftButtonUp) mouseState.setEvent(MouseEvent::LeftButtonUp);
        else mouseState.setEvent(MouseEvent::None);
    }
    else if (event == cv::EVENT_LBUTTONDBLCLK) {
        mouseState.setPosittion({ x, y });
        mouseState.setEvent(MouseEvent::LeftDoubleClick);
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        mouseState.setPosittion({ x, y });
        mouseState.setEvent(MouseEvent::RightButtonDown);
    }
    else if (event == cv::EVENT_RBUTTONUP) {
        mouseState.setPosittion({ x, y });
        if (mouseState.getEvent() != MouseEvent::RightButtonUp) mouseState.setEvent(MouseEvent::RightButtonUp);
        else mouseState.setEvent(MouseEvent::None);
    }
    else if (event == cv::EVENT_RBUTTONDBLCLK) {
        mouseState.setPosittion({ x, y });
        mouseState.setEvent(MouseEvent::RightDoubleClick);
    }
    else if (event == cv::EVENT_MBUTTONDOWN) {
        mouseState.setPosittion({ x, y });
        mouseState.setEvent(MouseEvent::MiddleButtonDown);
    }
    else if (event == cv::EVENT_MBUTTONUP) {
        mouseState.setPosittion({ x, y });
        if (mouseState.getEvent() != MouseEvent::MiddleButtonUp) mouseState.setEvent(MouseEvent::MiddleButtonUp);
        else mouseState.setEvent(MouseEvent::None);
    }
    else if (event == cv::EVENT_MOUSEWHEEL) {
        // Mouse wheel scroll
        int delta = cv::getMouseWheelDelta(flags);
        if (delta > 0) {
            // Scroll up
            mouseState.setPosittion({ x, y });
            mouseState.setEvent(MouseEvent::ScrollUp);
        }
        else {
            // Scroll down
            mouseState.setPosittion({ x, y });
            mouseState.setEvent(MouseEvent::ScrollDown);
        }
    }

}
