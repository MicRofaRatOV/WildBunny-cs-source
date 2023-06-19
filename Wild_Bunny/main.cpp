#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"

typedef unsigned long DWORD;

#include "bhop.h"

#include <iostream>
#include <thread>
#include <string>

using std::string;

const short frequency = 10; // milliseconds

const short WINDOW_WIDTH = 320;
const short WINDOW_HEIGHT = 150;
const float FREEZE_TIME = 0.f;

const string wbVersion = "0.2";

bool active = true;
bool kill = false;

sf::Color bgColor = { 0, 15, 5 };

short selectedVersion = 0;
namespace offsets // 0 - v9192, 1 - v34
{
    constexpr DWORD localPlayer[] = { 0x4C88E8, 0x4C6708 };
    constexpr DWORD flags[] = { 0x0350, 0x350 };
    constexpr DWORD forceJump[] = { 0x4F5D24, 0x4F3B3C };
}


int main()
{
    // First, I load the logo and create an sprite
    sf::Texture logo;
        
    if (!logo.loadFromFile("img/wildbunny.png")) {
        exit(1);
    }

    sf::Sprite sp;
    sp.setTexture(logo);
    sp.scale(0.25, 0.25); // Scale!!

    int logoWidth = sp.getGlobalBounds().width;
    int logoHeight = sp.getGlobalBounds().height;

    sf::RenderWindow welcomeWindow(sf::VideoMode(logoWidth, logoHeight), "Wild Bunny", sf::Style::None); // <- Important!! Style=None removes title

    sf::Clock timer;
    sf::Time time = timer.restart();

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            // event loop is always needed
        }
        // Window closed by time
        time += timer.restart();
        if (time >= sf::seconds(FREEZE_TIME)) {
            welcomeWindow.close();
        }

        welcomeWindow.clear();
        welcomeWindow.draw(sp);
        welcomeWindow.display();
    }

    std::thread bhopThread(bhop, std::ref(active), offsets::localPlayer[0], offsets::flags[0], offsets::forceJump[0], std::ref(kill), frequency);
    
    // Cheat staring here
    //
    //
    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Wild Bunny (CS:S)", sf::Style::Titlebar | sf::Style::Close);

    sf::Text textBhop, textCssVersion, textV92, textV34, textSleep, textVersion;
    sf::Font font;

    font.loadFromFile("font/Lato-Regular.ttf");

    textBhop.setFont(font);
    textBhop.setString("Enable bhop");
    textBhop.setCharacterSize(14);
    textBhop.setFillColor(sf::Color::White);
    textBhop.setPosition(sf::Vector2f{ 55, 45 });

    textCssVersion.setFont(font);
    textCssVersion.setString("CS:S version");
    textCssVersion.setCharacterSize(14);
    textCssVersion.setFillColor(sf::Color::White);
    textCssVersion.setPosition(sf::Vector2f{ 55, 85 });

    textV92.setFont(font);
    textV92.setString("92");
    textV92.setCharacterSize(14);
    textV92.setFillColor(bgColor);
    textV92.setPosition(sf::Vector2f{ 148, 86 });

    textV34.setFont(font);
    textV34.setString("34");
    textV34.setCharacterSize(14);
    textV34.setFillColor(bgColor);
    textV34.setPosition(sf::Vector2f{ 166, 86 });

    textSleep.setFont(font);
    textSleep.setString("Zzz");
    textSleep.setCharacterSize(14);
    textSleep.setFillColor(sf::Color::White);
    textSleep.setPosition(sf::Vector2f{ 194, 20 });

    textVersion.setFont(font);
    textVersion.setString("v"+wbVersion);
    textVersion.setCharacterSize(14);
    textVersion.setFillColor(sf::Color::White);
    textVersion.setPosition(sf::Vector2f{ WINDOW_WIDTH-40, WINDOW_HEIGHT-25 });

    auto boxEnable = sf::RectangleShape{ {16.f, 16.f} };
    boxEnable.setOutlineThickness(1);
    boxEnable.setOutlineColor(sf::Color::White);
    boxEnable.setPosition(148, 46);
    boxEnable.setFillColor(sf::Color::White);

    auto boxV92 = sf::RectangleShape{ {16.f, 16.f} };
    boxV92.setOutlineThickness(1);
    boxV92.setOutlineColor(sf::Color::White);
    boxV92.setPosition(148, 86);
    boxV92.setFillColor(sf::Color::White);

    auto boxV34 = sf::RectangleShape{ {16.f, 16.f} };
    boxV34.setOutlineThickness(1);
    boxV34.setOutlineColor(sf::Color::White);
    boxV34.setPosition(166, 86);
    boxV34.setFillColor(sf::Color::Transparent);

    sf::Texture bunny, wind;

    if (!bunny.loadFromFile("img/logo.png")) {
        exit(2); // error_2
    }

    if (!wind.loadFromFile("img/wind.png")) {
        exit(3); // error_3
    }

    sf::Sprite bunnySprite;
    bunnySprite.setTexture(bunny);
    bunnySprite.setPosition(215, 30);

    sf::Sprite windSprite;
    windSprite.setTexture(wind);
    windSprite.setPosition(223, 117);

    std::thread jumping([](bool &active, sf::Sprite &bunnySprite, bool &kill, sf::Sprite& windSprite)
        {
            while (!kill)
            {
                if (active)
                {
                    for (short i = 1; i < 10; i++)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                        bunnySprite.setPosition(215, 32 - i);
                        windSprite.setPosition(223, 117 - i);
                    }
                    for (short i = 10; i > 0 ; i--)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                        bunnySprite.setPosition(215, 32 - i);
                        windSprite.setPosition(223, 117 - i);
                    }
                    /*
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    bunnySprite.setPosition(215, 20);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    bunnySprite.setPosition(215, 30);
                    */
                }
            }
        }, std::ref(active), std::ref(bunnySprite), std::ref(kill), std::ref(windSprite));

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Enable/Disable bhop
            if (event.type == sf::Event::MouseButtonPressed) {
                if (boxEnable.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    active = !active;
                    if (active)
                    {
                        boxEnable.setFillColor(sf::Color::White);
                    }
                    else
                    {
                        boxEnable.setFillColor(bgColor);
                    }
                }
                // Set V92 [0]
                if (boxV92.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    selectedVersion = 0;
                    boxV92.setFillColor(sf::Color::White);
                    boxV34.setFillColor(bgColor);

                    textCssVersion.setString("CS:S version"); // Dont forget to remove after source 34 update
                    textCssVersion.setFillColor(sf::Color::White);
                }
                // Set V34 [1]
                if (boxV34.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    selectedVersion = 1;
                    boxV92.setFillColor(bgColor);
                    boxV34.setFillColor(sf::Color::White);

                    textCssVersion.setString("Coming soon"); // Dont forget to remove after source 34 update
                    textCssVersion.setFillColor(sf::Color::Red);
                }
            } 
        }

        // clear the window with bg color
        window.clear(bgColor);

        if (active)
        {
            window.draw(windSprite);
        }
        else
        {
            window.draw(textSleep);
        }

        window.draw(textBhop);
        window.draw(textCssVersion);
        window.draw(boxEnable);
        window.draw(boxV92);
        window.draw(boxV34);
        window.draw(textV92);
        window.draw(textV34);
        window.draw(bunnySprite);
        window.draw(textVersion);
        

        // end the current frame
        window.display();
    }

    // kill bhop thread
    kill = true;
    jumping.join();
    bhopThread.join();
    
	return 0;
}
