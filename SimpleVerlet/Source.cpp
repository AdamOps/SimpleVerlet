#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>

#include "Sphere.h"

using sf::RenderWindow;
using sf::Vector2f;

int main()
{
    RenderWindow window(sf::VideoMode(800, 600), "Verlet");
    window.setFramerateLimit(60);

    std::vector<Sphere> objectSet;

    int randGenNew;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time ElapsedTime = clock.getElapsedTime();
        if (ElapsedTime.asMilliseconds() > 100) {
            objectSet.push_back(Sphere());

            clock.restart();
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < objectSet.size(); i++) {
            objectSet.at(i).update();
            window.draw(objectSet.at(i).shape);
        }
        window.display();


    }

    return 0;
}