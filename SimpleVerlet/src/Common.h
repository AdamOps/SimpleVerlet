#pragma once
#include <SFML/Graphics.hpp>

float vertDistance(sf::Vector2f posOne, sf::Vector2f posTwo);

double vertAngle(sf::Vector2f posOne, sf::Vector2f posTwo);

float vertAtan2Angle(sf::Vector2f posOne, sf::Vector2f posTwo);

float vectorLength(sf::Vector2f vect);

float dotProduct(sf::Vector2f vectOne, sf::Vector2f vectTwo);