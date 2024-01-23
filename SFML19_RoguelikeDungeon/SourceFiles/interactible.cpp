/**
*
* File: interactible.cpp
* Description: Contains the implementation for the Interactible class.
*
*/

#include "interactible.h"
#include "texture_manager.h"

Interactible::Interactible() {
	setSize(sf::Vector2f(40, 40));
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::interactible);
}

Interactible::Interactible(int x, int y) : Interactible() {
	setPosition(x, y);
}

void Interactible::set_pos(int x, int y) { setPosition(x, y); }

int Interactible::get_pos(char z) { return (z == 'x') ? getPosition().x : getPosition().y; }

void Interactible::draw(sf::RenderWindow& window) { window.draw(*this); }