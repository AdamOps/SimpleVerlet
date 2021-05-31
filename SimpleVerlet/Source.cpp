#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#include "Sphere.h"
#include "Stick.h"

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
    std::vector<Stick> objectSticks;
    int sizeObjectSet = 0;
    bool constrained = false;

    srand(1);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                objectSet.push_back(Sphere(sizeObjectSet));
                sizeObjectSet++;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (constrained == false) {
                    constrained = true;
                    objectSticks.clear();
                    for (int n = 1; n < objectSet.size(); n++) {
                        int vertOne = n;
                        int vertTwo = n - 1;
                        objectSticks.push_back(Stick(n, n - 1, objectSet.at(vertOne).position, objectSet.at(vertTwo).position));
                        std::cout << "Stick made" << std::endl;
                        if (n % 4 == 0) {
                            int vertThree = n - 4;
                            objectSticks.push_back(Stick(vertOne, vertThree, objectSet.at(vertOne).position, objectSet.at(vertThree).position));
                            std::cout << "Structural stick made" << std::endl;
                        }
                    }

                    if (sizeObjectSet > 2 && sizeObjectSet % 4 > 0) {
                        int vertOne = sizeObjectSet - 1;
                        objectSticks.push_back(Stick(sizeObjectSet, 0, objectSet.at(vertOne).position, objectSet.at(1).position));
                    }
                }
                else {
                    constrained = false;
                    objectSticks.clear();
                }
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
            if (constrained == true) {
                objectSet.at(i).constraint();
            }
            window.draw(objectSet.at(i).shape);
        }


        window.display();


    }

    return 0;
}