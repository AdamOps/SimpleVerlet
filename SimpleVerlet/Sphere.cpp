#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Sphere.h"

Sphere::Sphere() {
	sf::CircleShape shape;
	Sphere::velocity.x = rand() % 22;
	Sphere::velocity.y = rand() % 22;

	int randRot = rand() % 22;

	Sphere::shape.setPosition(400, 300);
	Sphere::shape.setFillColor(sf::Color::Blue);
	Sphere::shape.setRadius(10);
	Sphere::shape.setOrigin(10, 10);

	Sphere::velocity.x = Sphere::velocity.x * cos(randRot) - velocity.y * sin(randRot);
	Sphere::velocity.y = Sphere::velocity.x * sin(randRot) - velocity.y * cos(randRot);
}

void Sphere::update() {
	Sphere::shape.move(Sphere::velocity);
}