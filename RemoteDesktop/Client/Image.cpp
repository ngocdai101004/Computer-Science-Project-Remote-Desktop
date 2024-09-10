#include "image.h"
#include "Mouse.h"
Image::Image() {

}

void Image::receiveImages(SOCKET clientSocket) {
    std::vector<uchar> receivedData;
    while (true) {
        int chunkSize;
        int receivedBytes = recv(clientSocket, reinterpret_cast<char*>(&chunkSize), sizeof(int), 0);


        if (receivedBytes != sizeof(int)) {
            std::cout << "Error receiving image size." << std::endl;
            break;
        }

        if (chunkSize == -1) {
            cv::Mat frame = cv::imdecode(receivedData, cv::IMREAD_COLOR);

            if (!frame.empty()) {
                cv::imshow("Virtual Machine Screen", frame);
                cv::setMouseCallback("Virtual Machine Screen", mouseEventControl, NULL);
                cv::Rect windowRect = cv::getWindowImageRect("Virtual Machine Screen");
                //std::cout << windowRect.width << " " << windowRect.height << std::endl;
                cv::waitKey(1);
                frame.release();
                receivedData.clear();
            }
        }
        else {
            std::vector<uchar> buffer(chunkSize);
            receivedBytes = recv(clientSocket, reinterpret_cast<char*>(buffer.data()), chunkSize, 0);

            //std::cout << chunkSize << " " << receivedBytes << std::endl;
            std::cout << " ";


            if (receivedBytes != chunkSize) {
                std::cout << "Error receiving image data." << std::endl;
                //break;
            }
            receivedData.insert(receivedData.end(), buffer.begin(), buffer.end());

        }
    }
}

void Image::threadImage(SOCKET clientSocket) {
    std::thread receiveThread(&Image::receiveImages, this, clientSocket);
    receiveThread.join();
}