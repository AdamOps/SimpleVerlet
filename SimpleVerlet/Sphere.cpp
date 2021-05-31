#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "Sphere.h"

float Sphere::gravity = 2.0;
float Sphere::elasticity = 0.50;

Sphere::Sphere(int sizeObjectSet) {
	Sphere::id = sizeObjectSet;

	Sphere::velocity.x = std::sin(rand()) * 30;
	Sphere::velocity.y = std::cos(rand()) * 30;

	float dirX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float dirY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	Sphere::oldPosition.x = 400 + (2*(dirX < 0.5) - 1)*velocity.x;
	Sphere::oldPosition.y = 300 + (2*(dirY < 0.5) - 1)*velocity.y;
	Sphere::position.x = 400;
	Sphere::position.y = 300;

	Sphere::randRot = rand() % 22;

	Sphere::shape.setPosition(400.0, 300.0);
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
	float dy = position.y - oldPosition.y;

	sf::Vector2f gravVector(0.f, gravity);

	sf::Vector2f gravPosition = position + gravVector;
	sf::Vector2f gravOldPosition;

	// If the particle crosses a border, there are two useful distances to track: B shows how far beyond the border the particle has gone; A shows how much it covered before reaching the border.
	float A, B;

		if (position.x + dx > width) {
			// In this case, the particle is running off the right side of the screen (exceeding its width).
			// The distance to the border (A) is the width of the screen - the current position.x of the particle.
			// E.g. if the width is 800, and the position.x is 750, then the particle is clearly 50 units away from the border.
			A = width - position.x;

			// The distance beyond the border is the expected position.x after movement (dx) minus the border.
			// Hence, if the width is 800, the position.x is 750 and the dx is 150, then the particle is expected to move to (750 + 150) - 800 = 100 units beyond the border.
			// These 100 units should not be covered, but instead be reversed in the opposite direction. Hence, B serves as an indication of how far the particle should bounce back (baseline without any dampening/forces)
			B = position.x + dx - width;

			// Need to move the oldPosition.x around, so that it will look like the particle came from the wall. This oldPosition.x should be the mirror image of the current position (prior to movement dx) along the wall that the particle is expected to strike.
			// Since position.x is A units away from the border, if oldPosition.x is position.x plus 2*A, it will look as if oldPosition.x is a mirror image of position.x along the wall.
			// If the width is 800, position.x is 750 and dx is 150, then it should seem as if the particle was coming from an x of 850, moving in the opposite direction by (A+B)=dx, where only the B part will remain visible.
			oldPosition.x = position.x + 2 * A;

			// Now, position.x itself can be updated. Since the particle still covers a distance of dx in total, it needs to move by A in one direction, and then B in the other direction.
			// Can skip the movement in the A direction, since it'll immediately be undone, and change position.x in the opposite direction by B.
			position.x = width - B;

			// In this part, the particle is bouncing off the right wall, so that means it should end up at B units away from the border.
			// If the width is 800, the position.x is 750 and dx is 150, then the particle should end up at an x of (800 - B) = 800 - 100 = 700.

			// The particle should in the end only move somewhat to the left compared to its original position.
			// If width is 800, position.x is 750 and dx is 150, then to move it to an x of 700, the particle needs to move (A - B) = (50 - 100) = -50.
			// Negative amounts entail leftward movement, so this is all correct now.
			dx = A - B;
		}
		else if (position.x + dx < 0) {
			// Particle runs off the left side. Let position.x be 50 and dx is -150, then the particle would end up at x of -100. It should end up at +100 instead.
			// Distance to the border (A) is 0 - position.x = -position.x. Negative number implies it's to the left.
			A = -position.x;

			// Distance it would cover beyond the border is dx - A. With the prior numbers, (dx - A) = (-150 -(-50)) = -100
			B = dx - A;

			// Old position needs to be updated first again. For future reference, need to pretend that the particle was coming from the left all along.
			// With above numbers, it should look as if it's coming from a position of -50, so that when the particle is done traveling a dx of 150 it ends up at +100.
			oldPosition.x = position.x + 2 * A;

			// With prior numbers, particle should move 50 left and 100 right, so 50 right overall. Position should then be (A - B) = (-50 -(-100)) = 50
			position.x = -B;

			dx = A - B;
		}
		else {
			oldPosition.x = position.x;
			position.x += dx;
		}

		if (position.y + dy > height) {
			// Particle is running off the bottom of the screen. Logic is the same as when it runs off the right.
			// If the height is 600, and the position.y is 550, then the particle is 50 units away from the border.
			A = height - position.y;

			// Let height be 600, position.y be 550 and dy be 150. Then the particle will travel (550 + 150) - 600 = 100 units too far.
			B = position.y + dy - height;
			
			// Update old position vector to make it a mirror image again.
			oldPosition.y = position.y + 2 * A;

			// New position should be based on the distance traveled in reverse from the border, so height - B. With the example numbers here that's 600 - 100 = 500
			position.y = height - B;

			// Actual distance traveled is here A - B = 50 - 100 = -50, so that position.y + dy = 500.
			dy = A - B;
		}
		else if (position.y + dy < 0) {
			A = -position.y;
			B = dy - A;

			oldPosition.y = position.y + 2 * A;
			position.y = -B;

			dy = A - B;
		}
		else {
			oldPosition.y = position.y;
			position.y += dy;
		}

	shape.move(dx, dy);
}
