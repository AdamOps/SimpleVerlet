#include <SFML/Graphics.hpp>
#include <cmath>

#include "Common.h"

float vertDistance(sf::Vector2f posOne, sf::Vector2f posTwo) {
	return sqrt((posOne.x - posTwo.x) * (posOne.x - posTwo.x) + (posOne.y - posTwo.y) * (posOne.y - posTwo.y));
}

double vertAngle(sf::Vector2f posOne, sf::Vector2f posTwo) {
	float height = posTwo.y - posOne.y;
	float base = posTwo.x - posOne.x;
	return atan(height / base);
}

float vertAtan2Angle(sf::Vector2f posOne, sf::Vector2f posTwo) {
	float dot = dotProduct(posOne, posTwo);
	float det = posOne.x * posTwo.y - posOne.y * posTwo.x;
	float angle = atan2(det, dot);
	return angle;
}

float vectorLength(sf::Vector2f vect) {
	return sqrt(vect.x * vect.x + vect.y * vect.y);
}

float dotProduct(sf::Vector2f vectOne, sf::Vector2f vectTwo) {
	return vectOne.x * vectTwo.x + vectOne.y * vectTwo.y;
}