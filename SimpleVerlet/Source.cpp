#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#include "Sphere.h"

using sf::RenderWindow;
using sf::Vector2f;

int main()
{
    RenderWindow window(sf::VideoMode(800, 600), "Verlet");
    window.setFramerateLimit(60);

    std::vector<Sphere> objectSet;
    int sizeObjectSet = 0;

    srand(1);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time ElapsedTime = clock.getElapsedTime();
        sf::Time drawTime = clock.getElapsedTime();
        if (ElapsedTime.asMilliseconds() > 500 && objectSet.size() < 4) {
            objectSet.push_back(Sphere(sizeObjectSet));
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
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