/**
*
* File: shop.cpp
* Description: Contain implementations for the Shop class.
*
*/

#include "shop.h"
#include "texture_manager.h"

Shop::Shop() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::White);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::shop);
}

Shop::Shop(int x, int y) : Shop() {
	setPosition(x, y);
}