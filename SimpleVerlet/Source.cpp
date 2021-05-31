#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#include "Sphere.h"

using sf::RenderWindow;
using sf::Vector2f;

const float zoomSpeed(0.02);

int main()
{
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    RenderWindow window(sf::VideoMode(800, 600), "Verlet");
    window.setFramerateLimit(60);
    window.setView(view);
    float viewZoom = 1;
    bool panning = false;
    Vector2f panStart;

    std::vector<Sphere> objectSet;
    int sizeObjectSet = 0;

    srand(1);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time ElapsedTime = clock.getElapsedTime();
        sf::Time drawTime = clock.getElapsedTime();
        if (ElapsedTime.asMilliseconds() > 500 && objectSet.size() < 15) {
            objectSet.push_back(Sphere(sizeObjectSet));
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseWheelMoved) {
                view.zoom(viewZoom - zoomSpeed * event.mouseWheel.delta);
                window.setView(sf::View(view));
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                std::cout << "pressing" << std::endl;
                panning = true;
                panStart.x = sf::Mouse::getPosition(window).x;
                panStart.y = sf::Mouse::getPosition(window).y;
            }

            if (panning == true) {
                std::cout << "panning" << std::endl;
                view.move(panStart.x - sf::Mouse::getPosition(window).x, panStart.y - sf::Mouse::getPosition(window).y);
                window.setView(sf::View(view));
                panStart.x = sf::Mouse::getPosition(window).x;
                panStart.y = sf::Mouse::getPosition(window).y;
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                panning = false;
            }
        }

        window.clear();

        for (int i = 0; i < objectSet.size(); i++) {
            objectSet.at(i).update(&window);
            window.draw(objectSet.at(i).shape);
        }
        window.display();


    }

    return 0;
}