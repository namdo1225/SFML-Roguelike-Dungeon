/**
*
* File: stair.cpp
* Description: Contain implementations for the Stair class.
*
*/

#include "Floor/stair.h"
#include "Manager/texture_manager.h"

Stair::Stair() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::Yellow);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::stair);
}

Stair::Stair(int x, int y) : Stair() {
	setPosition(x, y);
}