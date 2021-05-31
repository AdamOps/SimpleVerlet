#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Sphere
{
public:
	Sphere();

	sf::Vector2f position,
		oldPosition,
		velocity,
		acceleration,
		force;

	sf::CircleShape shape;

	void update();

private:
	int randRot;
};

