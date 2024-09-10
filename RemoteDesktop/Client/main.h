#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <utility>
#include <thread>
#include <conio.h>
#include "Mouse.h"


// Declared constant variable
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Init button
const std::string font1 = "Angeline Vintage_Demo.ttf";
const float button_width = 300.f;
const float button_height = 60.f;
const sf::Color cRect = sf::Color::White;
const std::string text_init = "Text";
const sf::Color cText = sf::Color(0, 0, 0);
const float padding = 10.f;

// Header const
const float header_width = SCREEN_WIDTH;
const float header_height = 100.f;
const std::string header_text = "Remote Desktop";
const sf::Color header_cRect = sf::Color(97, 103, 122);
const sf::Color header_cText = sf::Color(255, 255, 255);

// Button server const
const std::string server_text = "Server";
const sf::Color serverOff_cRect = sf::Color(205, 245, 253);
const sf::Color serverOn_cRect = sf::Color(0, 169, 255);
const sf::Color serverOff_cText = sf::Color(97, 103, 122);
const sf::Color serverOn_cText = sf::Color(0, 0, 0);

// Button item const
const float padding_items = 30.f;
const sf::Color itemOff_cRect = sf::Color(255, 248, 201);
const sf::Color itemOn_cRect = sf::Color(246, 250, 112);
const sf::Color itemOff_cText = sf::Color(97, 103, 122);
const sf::Color itemOn_cText = sf::Color(0, 0, 0);

// Button Ip + Port const
const sf::Color infoServer_cRect = sf::Color(0, 0, 0);
const sf::Color infoServer_cText = sf::Color(255, 255, 255);

// Button connected const
const float connected_width = 200.f;
const sf::Color connectedOff_cRect = sf::Color(255, 255, 255);
const sf::Color connectedOn_cRect = sf::Color(183, 4, 4);
const sf::Color connectedOff_cText = sf::Color(0, 0, 0);
const sf::Color connectedOn_cText = sf::Color(255, 255, 255);
const std::string connected_text = "Connected";

// Button return const
const float return_width = 200.f;
const sf::Color return_cRect = sf::Color(183, 4, 4);
const sf::Color return_cText = sf::Color(255, 255, 255);
const std::string return_text = "Return";

// Text ip + port
const float distance = 300.f; // distance between text and button ip + port
const std::string text_ip = "IP:";
const std::string text_port = "PORT:";
const sf::Color text_color = sf::Color(0, 0, 0);

// Font
const std::string fontStandardPath = "Swansea-q3pd.ttf";
const std::string fontBoldPath = "SwanseaBold-D0ox.ttf";
const std::string fontItalicPath = "SwanseaItalic-AwqD.ttf";
const std::string fontItalicBoldPath = "SwanseaBoldItalic-p3Dv.ttf";

extern Mouse mouseState;