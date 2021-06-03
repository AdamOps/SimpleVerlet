#include <vector>
#include <cmath>
#include <iostream>
#include <set>

#include "Sphere.h"
#include "Stick.h"
#include "Common.h"

Stick::Stick(int vertexOne, int vertexTwo, sf::Vector2f vertOnePos, sf::Vector2f vertTwoPos) {
	distVertices = vectorLength(vertTwoPos - vertOnePos);
	constraintDist = distVertices;
	vertOne = vertOnePos;
	vertTwo = vertTwoPos;

	idOne = vertexOne;
	idTwo = vertexTwo;

	shape.setSize(sf::Vector2f(distVertices, 3));
	shape.setPosition(vertOnePos.x, vertOnePos.y);
	shape.setRotation(atan2(vertOnePos.x * vertTwoPos.x + vertOnePos.y * vertTwoPos.y, vertOnePos.x * vertTwoPos.y - vertOnePos.y * vertTwoPos.x));
}

void Stick::update(sf::Vector2f newPosOne, sf::Vector2f newPosTwo, bool currConstrained) {

	if (currConstrained == false) {
		distVertices = vectorLength(newPosTwo - newPosOne);
		shape.setSize(sf::Vector2f(distVertices, 3));
		constraintDist = distVertices;
	}

	if (newPosOne.x < newPosTwo.x) {
		shape.setPosition(newPosOne.x, newPosOne.y);
	}
	else {
		shape.setPosition(newPosTwo.x, newPosTwo.y);
	}

	shape.setRotation(vertAngle(newPosOne, newPosTwo) * 180/3.14159);

}

// Requires the set of spheres (reference), so that their positions can be modified. Everything else is already stored in the Stick class.
void Stick::constrainVerts(std::vector<Sphere>& objectSet, float height, float width) {
	// Get current position of the vertices connected by the sticks.
	sf::Vector2f vertOne = objectSet.at(idOne).position;
	sf::Vector2f vertTwo = objectSet.at(idTwo).position;


	// Calculate the differences in the X and the Y values.
	float diffX = vertOne.x - vertTwo.x;
	float diffY = vertOne.y - vertTwo.y;
	float currDist = vertDistance(vertOne, vertTwo);
	float excessDist = constraintDist - currDist;
	float excessPercent = (excessDist / currDist) / 2;
	float offsetX = excessPercent * diffX;
	float offsetY = excessPercent * diffY;

	std::cout << "offsetX: " << offsetX << ", offsetY: " << offsetY << std::endl;
	// Update the positions of the vertices and change the Verlet position storage.

	if (vertOne.x + offsetX > width) {
		offsetX = width - vertOne.x;
	}
	else if (vertOne.x + offsetX < 0) {
		offsetX = -vertOne.x;
	}
	if (vertOne.y + offsetY > height) {
		offsetY = height - vertOne.y;
	}
	else if (vertOne.y + offsetY < 0) {
		offsetY = -vertOne.y;
	}
		
		objectSet.at(idOne).oldPosition.x += offsetX;
		objectSet.at(idOne).oldPosition.y += offsetY;
		objectSet.at(idOne).position.x += offsetX;
		objectSet.at(idOne).position.y += offsetY;
		
		objectSet.at(idTwo).oldPosition.x -= offsetX;
		objectSet.at(idTwo).oldPosition.y -= offsetY;
		objectSet.at(idTwo).position.x -= offsetX;
		objectSet.at(idTwo).position.y -= offsetY;

		objectSet.at(idOne).shape.move(offsetX, offsetY);
		objectSet.at(idTwo).shape.move(-offsetX, -offsetY);

		
}