#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <set>

#include "Sphere.h"

class Stick
{
public:
	Stick(int vertexOne, int vertexTwo, sf::Vector2f vertOnePos, sf::Vector2f vertTwoPos);

	sf::RectangleShape shape;

	float distVertices;
	float constraintDist;

	sf::Vector2f vertOne, vertTwo;
	int idOne, idTwo;

	void update(sf::Vector2f newPosOne, sf::Vector2f newPosTwo, bool currConstrained);
	void constrainVerts(std::vector<Sphere>& objectSet);
};

