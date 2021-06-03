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
void Stick::constrainVerts(std::vector<Sphere>& objectSet) {
		// Get current position of the vertices connected by the sticks.
		sf::Vector2f vertOne = objectSet.at(idOne).position;
		sf::Vector2f vertTwo = objectSet.at(idTwo).position;

		
		// Calculate the differences in the X and the Y values.
		float diffX = vertOne.x - vertTwo.x;
		float diffY = vertOne.y - vertTwo.y;
		float currDist = vertDistance(vertOne, vertTwo);
		float excessDist = constraintDist - currDist;
		float excessPercent = (excessDist / currDist) / 2;

		
		// Move each vertex by 50% of the excess distance
		sf::Vector2f newVertOne(vertOne.x - (diffX * excessPercent), vertOne.y - (diffY * excessPercent));
		sf::Vector2f newVertTwo(vertTwo.x + (diffX * excessPercent), vertTwo.y + (diffY * excessPercent));

		std::cout << excessPercent << std::endl;

		/*
		// Update the positions of the vertices and change the Verlet position storage.
		objectSet.at(idOne).shape.setPosition(newVertOne);
		objectSet.at(idOne).oldPosition = vertOne;
		objectSet.at(idOne).position = newVertOne;

		
		objectSet.at(idTwo).shape.setPosition(newVertTwo);
		objectSet.at(idTwo).oldPosition = vertTwo;
		objectSet.at(idTwo).position = newVertTwo;
		*/
}