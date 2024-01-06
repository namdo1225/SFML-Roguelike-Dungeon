/**
*
* File: collectible_class.cpp
* Description: Contain implementations for the Collectible class.
*
*/

#include "collectible_class.h"

sf::Texture Collectible::col_tex;

Collectible::Collectible() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::Yellow);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&col_tex);
}

Collectible::Collectible(int x, int y, unsigned int t_id) : Collectible() {
	id = t_id;
	setPosition(x, y);
}

int Collectible::get_pos(char z) { return z == 'x' ? getPosition().x : getPosition().y; }

unsigned int Collectible::get_id() { return id; }

void Collectible::set_pos(int x, int y) { setPosition(x, y); }

void Collectible::draw(sf::RenderWindow& window) { window.draw(*this); }

bool Collectible::load_texture() {
	return col_tex.loadFromFile("Texture\\GG_04_Items.png");
}