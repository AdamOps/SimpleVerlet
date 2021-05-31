#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Sphere
{
public:
	Sphere(int sizeObjectSet);

	static float gravity;
	static float elasticity;
	static float thresh;
	static float friction;

	sf::Vector2f position,
		oldPosition,
		velocity,
		acceleration,
		force;

	sf::CircleShape shape;

	int id;

	void update(sf::RenderWindow *window);
	void constraint(sf::Vector2f vertOnePos, sf::Vector2f verTwoPos);

	int randRot;
};

