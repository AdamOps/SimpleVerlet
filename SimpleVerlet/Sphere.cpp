#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "Sphere.h"

Sphere::Sphere() {
	/*sf::CircleShape shape;*/

	Sphere::velocity.x = std::sin(rand())*5;
	Sphere::velocity.y = std::cos(rand())*5;

	float dirX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float dirY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	Sphere::oldPosition.x = 400 + (2*(dirX < 0.5) - 1)*velocity.x;
	Sphere::oldPosition.y = 300 + (2*(dirY < 0.5) - 1)*velocity.y;
	Sphere::position.x = 400;
	Sphere::position.y = 300;

	Sphere::randRot = rand() % 22;

	Sphere::shape.setPosition(400, 300);
	Sphere::shape.setFillColor(sf::Color::Blue);
	Sphere::shape.setRadius(10);
	Sphere::shape.setOrigin(10, 10);

	velocity.x = velocity.x * cos(randRot) - velocity.y * sin(randRot);
	velocity.y = velocity.x * sin(randRot) - velocity.y * cos(randRot);
}

void Sphere::update(sf::Vector2f velocity) {
	float vx = position.x - oldPosition.x;
	float vy = position.y - oldPosition.y;

	float A, B;

	oldPosition.x = position.x;
	position.x += vx;

	oldPosition.y = position.y;
	position.y += vy;

	bool wrongX = (position.x + vx > 800 || position.x + vx < 0);
	bool wrongY = (position.y + vy > 600 || position.y + vy < 0);

	while (wrongX) {
		if (position.x + vx > 800) {
			A = 800 - position.x;
			B = position.x + vx - 800;
			oldPosition.x = position.x + 2 * A;
			position.x = 800 - B;

			vx = position.x - oldPosition.x;
		}

		if (position.x + vx <= 0) {
			A = position.x;
			B = -vx - A;
			oldPosition.x = position.x - 2 * A;
			position.x = B;

			vx = position.x - oldPosition.x;
		}

		if (position.x > 0 && position.x < 800) {
			wrongX = false;
		}
	}

	while (wrongY) {
		if (position.y + vy > 600) {
			A = 600 - position.y;
			B = position.y + vy - 600;
			oldPosition.y = position.y + 2 * A;
			position.y = 600 - B;

			vy = position.x - oldPosition.x;
		}
		else if (position.y + vy <= 0) {
			A = position.y;
			B = -vy - A;
			oldPosition.y = position.y - 2 * A;
			position.y = B;

			vy = position.y - oldPosition.y;
		}

		if (position.y > 0 && position.y < 600) {
			wrongY = false;
		}
	}

	std::cout << "oldX = " << oldPosition.x << ", oldY = " << oldPosition.y << std::endl;
	std::cout << "X = " << position.x << ", Y = " << position.y << std::endl;

	shape.move(vx, vy);
}

void Sphere::applyForces() {

}