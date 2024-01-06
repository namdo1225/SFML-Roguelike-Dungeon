/**
*
* File: shop_class.cpp
* Description: Contain implementations for the Shop class.
*
*/

#include "shop_class.h"

sf::Texture Shop::shop_tex;

Shop::Shop() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::White);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&shop_tex);
}

Shop::Shop(int x, int y) : Shop() {
	setPosition(x, y);
}

bool Shop::load_texture() {
	return shop_tex.loadFromFile("Texture\\GG_06_Shop.jpg");
}