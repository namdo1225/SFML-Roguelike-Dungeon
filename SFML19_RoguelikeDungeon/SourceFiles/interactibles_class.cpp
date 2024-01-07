/**
*
* File: interactibles_class.cpp
* Description: Contains the implementation for the Interactible class.
*
*/

#include "interactibles_class.h"

sf::Texture Interactible::tex;

Interactible::Interactible() {
	setSize(sf::Vector2f(40, 40));
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&tex);
}

Interactible::Interactible(int x, int y) : Interactible() {
	setPosition(x, y);
}

void Interactible::set_pos(int x, int y) { setPosition(x, y); }

int Interactible::get_pos(char z) { return (z == 'x') ? getPosition().x : getPosition().y; }

void Interactible::draw(sf::RenderWindow& window) { window.draw(*this); }

bool Interactible::load_texture() {
	return tex.loadFromFile("Texture\\GG_10_Interactible.jpg");
}