#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "Sphere.h"

float Sphere::gravity = 0.0;
float Sphere::elasticity = 0.50;
float Sphere::thresh = 0.5;
float Sphere::friction = 0.995;

Sphere::Sphere(int sizeObjectSet) {
	Sphere::id = sizeObjectSet;

	Sphere::velocity.x = 0; // std::sin(rand()) * 3;
	Sphere::velocity.y = std::cos(rand()) * 3;

	float dirX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float dirY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	float offsetX = static_cast<float>(rand()) / static_cast<float>(800 / 2);

	Sphere::oldPosition.x = 400 + (2*(dirX < 0.5) - 1)*velocity.x + offsetX;
	Sphere::oldPosition.y = 300 + (2*(dirY < 0.5) - 1)*velocity.y;
	Sphere::position.x = 400 + offsetX;
	Sphere::position.y = 300;

	Sphere::randRot = rand() % 22;

	Sphere::shape.setPosition(position.x, 300.0);
	Sphere::shape.setFillColor(sf::Color::Blue);
	Sphere::shape.setRadius(10);
	Sphere::shape.setOrigin(10, 10);

	velocity.x = velocity.x * cos(randRot) - velocity.y * sin(randRot);
	velocity.y = velocity.x * sin(randRot) - velocity.y * cos(randRot);
}

void Sphere::update(sf::RenderWindow *window) {
	// Get the window height and width, to determine whether a particle is going outside the borders.
	float width = (*window).getSize().x;
	float height = (*window).getSize().y;

	// Floats denoting the distance to be covered in the x-direction (dx) and y-direction (dy).
	float dx = position.x - oldPosition.x;
	float dy = position.y - oldPosition.y + gravity;

	// If the particle crosses a border, there are two useful distances to track: B shows how far beyond the border the particle has gone; A shows how much it covered before reaching the border.
	float A, B;

		if (position.x + dx > width) {
			A = width - position.x;
			B = position.x + dx - width;
			oldPosition.x = position.x + 2 * A;
			position.x = width - B;
			dx = A - B;
		}
		else if (position.x + dx < 0) {
			A = -position.x;
			B = dx - A;

			oldPosition.x = position.x + 2 * A;
			position.x = -B;

			dx = A - B;
		}
		else {
			oldPosition.x = position.x;
			position.x += dx;
		}

		if (position.y + dy > height) {
			A = height - position.y;
			B = position.y + dy - height - gravity;
			
			oldPosition.y = position.y + 2 * A;
			position.y = height - B;

			dy = A - B;
		}
		else if (position.y + dy < 0) {
			A = -position.y;
			B = dy - A + gravity;

			oldPosition.y = position.y + 2 * A;
			position.y = -B;

			dy = A - B;
		}
		else {
			oldPosition.y = position.y;
			position.y += dy;
		}

		if (position.y > height - thresh && oldPosition.y > height - thresh && dy < thresh) {
			dy = 0;
			position.y = height;
			oldPosition.y = height;
			position.x = position.x + (1 - friction )* (oldPosition.x - position.x);
			dx *= friction;
		}

	shape.move(dx, dy);
}
