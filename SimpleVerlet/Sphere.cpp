#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "Sphere.h"

// Static floats determining properties relevant for spheres.
float Sphere::gravity = 0.2;
float Sphere::elasticity = 0.9;
float Sphere::thresh = 1;
float Sphere::friction = 0.999;

Sphere::Sphere(int sizeObjectSet) {
	Sphere::id = sizeObjectSet;

	Sphere::velocity.x = std::sin(rand()) * 3;
	Sphere::velocity.y = std::cos(rand()) * 3;

	float dirX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float dirY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	float offsetX = static_cast<float>(rand()) / static_cast<float>(800 / 2);

	Sphere::oldPosition.x = 400 + (2*(dirX < 0.5) - 1)*velocity.x + offsetX;
	Sphere::oldPosition.y = 300 + (2*(dirY < 0.5) - 1)*velocity.y;
	Sphere::position.x = 400 + offsetX;
	Sphere::position.y = 300;

	Sphere::shape.setPosition(position.x, 300.0);
	Sphere::shape.setFillColor(sf::Color::Blue);
	Sphere::shape.setRadius(10);
	Sphere::shape.setOrigin(10, 10);

}

void Sphere::update(sf::RenderWindow* window) {
	// Get the window height and width, to determine whether a particle is going outside the borders.
	float width = (*window).getSize().x;
	float height = (*window).getSize().y;

	// Floats denoting the distance to be covered in the x-direction (dx) and y-direction (dy).
	float dx = (position.x - oldPosition.x)*friction;
	float dy = (position.y - oldPosition.y)*friction;

	oldPosition.x = position.x;
	oldPosition.y = position.y;
	position.x += dx;
	position.y += dy;

	position.y += gravity;

}

void Sphere::constrainPoints(sf::RenderWindow * window){
	float width = (*window).getSize().x;
	float height = (*window).getSize().y;

	float dx = position.x - oldPosition.x;
	float dy = position.y - oldPosition.y;

	// Less accurate version, but simpler. Baseline, to test whether the rest makes sense.
	/*
	if (position.x > width) {
		position.x = width;
		oldPosition.x = position.x + dx * elasticity;
	}
	else if (position.x < 0) {
		position.x = 0;
		oldPosition.x = position.x + dx * elasticity;
	}
	if (position.y > height) {
		position.y = height;
		oldPosition.y = position.y + dy * elasticity;
	}
	else if (position.y < 0) {
		position.y = 0;
		oldPosition.y = position.y + dy * elasticity;
	}
	*/
	
	// If the particle crosses a border, there are two useful distances to track: B shows how far beyond the border the particle has gone; A shows how much it covered before reaching the border. Both the position and the oldPosition need to be adjusted based on such collisions, to set up for the subsequent frame.
	float A, B;
		if (position.x > width) {
			A = width - position.x;
			B = position.x + dx * elasticity - width;
			oldPosition.x = position.x + 2 * A;
			position.x = width - B;
			dx = A - B;
		}
		else if (position.x < 0) {
			A = -position.x;
			B = dx * elasticity - A;
			oldPosition.x = position.x + 2 * A;
			position.x = -B;
			dx = A - B;
		}

		if (position.y > height) {
			A = height - position.y;
			B = position.y + dy * elasticity - height - gravity;
			oldPosition.y = position.y + 2 * A;
			position.y = height - B;
			dy = A - B;
		}
		else if (position.y < 0) {
			A = -position.y;
			B = dy * elasticity - A + gravity;
			oldPosition.y = position.y + 2 * A;
			position.y = -B;
			dy = A - B;
		}
		
}
