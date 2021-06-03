#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "Sphere.h"
#include "Stick.h"

using sf::RenderWindow;
using sf::Vector2f;

const float Pi = 3.141592653;

const float zoomSpeed(0.02);
const int vertPerObject(2);
const int constrainIterations(1);

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
    std::vector<int> stickIndices;
    bool alreadyPicked;

    int sizeObjectSet = 0;
    bool drawSticks = false;
    bool currConstrained = false;

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
                if (drawSticks == false && objectSet.size() >= vertPerObject) {
                    std::cout << "Generating sticks" << std::endl;
                    drawSticks = true;
                    objectSticks.clear();
                    stickIndices.clear();

                    while (stickIndices.size() < vertPerObject) {
                        int chosenVert = rand() % objectSet.size();
                        std::cout << "chosenVert: " << chosenVert << std::endl;
                        if (std::find(stickIndices.begin(), stickIndices.end(), chosenVert) == stickIndices.end()) {
                            stickIndices.push_back(chosenVert);
                        }
                    }

                    std::cout << "Created " << stickIndices.size() << " sticks. " << std::endl;
                    for (int i = 0; i < vertPerObject; i++) {
                        int vertOne = stickIndices.at(i);
                        int vertTwo = stickIndices.at((i + 1) % vertPerObject);
                        objectSticks.push_back(Stick(vertOne, vertTwo, objectSet.at(vertOne).position, objectSet.at(vertTwo).position));
                    }

                    // Commented code is to generate sticks between all vertices. Uncommented version does it between a random selection of vertices instead.
                    // 
                    //for (int n = 0; n < objectSet.size() - 1; n++) {
                    //    int vertOne = n;
                    //    int vertTwo = n + 1;
                    //    objectSticks.push_back(Stick(n, n + 1, objectSet.at(vertOne).position, objectSet.at(vertTwo).position));
                    //    std::cout << "Stick made" << std::endl;
                    //    if (n % 4 == 0 && n >= 4) {
                    //        int vertThree = n - 4;
                    //        objectSticks.push_back(Stick(vertOne, vertThree, objectSet.at(vertOne).position, objectSet.at(vertThree).position));
                    //        std::cout << "Structural stick made" << std::endl;
                    //    }
                    //}

                    //if (sizeObjectSet > 2) {
                    //    int vertOne = sizeObjectSet - 1;
                    //    std::cout << "pos of " << 0 << ": " << objectSet.at(0).position.x << ", " << objectSet.at(0).position.y << std::endl;
                    //    std::cout << "pos of " << vertOne << ": " << objectSet.at(vertOne).position.x << ", " << objectSet.at(vertOne).position.y <<  std::endl;
                    //    objectSticks.push_back(Stick(vertOne, 0, objectSet.at(vertOne).position, objectSet.at(0).position));
                    //}
                }
                else if (drawSticks == true) {
                    std::cout << "Sticks erased" << std::endl;
                    drawSticks = false;
                    objectSticks.clear();
                    stickIndices.clear();
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && drawSticks == true) {
                if (currConstrained == false) {
                    std::cout << "Adding stick constraint" << std::endl;
                }
                else {
                    std::cout << "Removing stick constraint" << std::endl;
                }
                currConstrained = (1 - currConstrained);
            }

            if (event.type == sf::Event::MouseWheelMoved) {
                view.zoom(viewZoom - zoomSpeed * event.mouseWheel.delta);
                window.setView(sf::View(view));
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                panning = true;
                panStart.x = sf::Mouse::getPosition(window).x;
                panStart.y = sf::Mouse::getPosition(window).y;
            }

            if (panning == true) {
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

        // First, update all vertices
        for (int i = 0; i < objectSet.size(); i++) {
            objectSet.at(i).update(&window);
        }

        // Next, update all sticks and apply constraints several times.
        if (drawSticks == true) {
            for (int iter = 0; iter < constrainIterations; iter++) {
                for (int j = 0; j < objectSticks.size(); j++) {
                    objectSticks.at(j).update(objectSet.at(objectSticks.at(j).idOne).position, objectSet.at(objectSticks.at(j).idTwo).position, currConstrained);
                    if (currConstrained == true) {
                        objectSticks.at(j).constrainVerts(objectSet);
                    }
                }
            }
        }
        // Finally, draw all points and all sticks
        for (int i = 0; i < objectSet.size(); i++) {
            window.draw(objectSet.at(i).shape);
        }

        if (drawSticks == true) {
            for (int j = 0; j < objectSticks.size(); j++) {
                window.draw(objectSticks.at(j).shape);
            }
        }

        // define a 100x100 square, red, with a 10x10 texture mapped on it
        //sf::Vertex vertices[] =
        //{
        //    sf::Vertex(sf::Vector2f(0,   0), sf::Color::Red, sf::Vector2f(0,  0)),
        //    sf::Vertex(sf::Vector2f(0, 100), sf::Color::Red, sf::Vector2f(0, 10)),
        //    sf::Vertex(sf::Vector2f(100, 100), sf::Color::Red, sf::Vector2f(10, 10)),
        //    sf::Vertex(sf::Vector2f(100,   0), sf::Color::Red, sf::Vector2f(10,  0))
        //};
        //// draw it
        //window.draw(vertices, 4, sf::Quads);


        window.display();


    }

    return 0;
}