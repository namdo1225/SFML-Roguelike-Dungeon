/**
*
* File: stair_class.cpp
* Description: Contain implementations for the Stair class.
*
*/

#include "stair_class.h"

sf::Texture Stair::stair_tex;

Stair::Stair() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::Yellow);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&stair_tex);
}

Stair::Stair(int x, int y) : Stair() {
	setPosition(x, y);
}

bool Stair::load_texture() {
	return stair_tex.loadFromFile("Texture\\GG_05_Stair.png");
}