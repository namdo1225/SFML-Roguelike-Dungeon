/**
*
* File: collectible.cpp
* Description: Contain implementations for the Collectible class.
*
*/

#include "Floor/collectible.h"
#include "Manager/texture_manager.h"

Collectible::Collectible() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::Yellow);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::collectible);
}

Collectible::Collectible(int x, int y, unsigned int t_id) : Collectible() {
	id = t_id;
	setPosition(x, y);
}

int Collectible::get_pos(char z) { return z == 'x' ? getPosition().x : getPosition().y; }

unsigned int Collectible::get_id() { return id; }

void Collectible::set_pos(int x, int y) { setPosition(x, y); }

void Collectible::draw(sf::RenderWindow& window) { window.draw(*this); }