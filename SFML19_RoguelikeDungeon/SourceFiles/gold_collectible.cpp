/**
*
* File: gold_collectible.cpp
* Description: Contain implementations for the Gold_Collectible class.
*
*/

#include "gold_collectible.h"
#include "texture_manager.h"

Gold_Collectible::Gold_Collectible() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::Yellow);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::gold);
}

Gold_Collectible::Gold_Collectible(unsigned int floor, unsigned int t_amount, int x, int y) : Gold_Collectible() {
	amount = (floor != 0) ? rand() % (5 * floor) + (floor * 0.25) + 1 : t_amount;
	setPosition(x, y);
}

unsigned int Gold_Collectible::get_amount() { return amount; }

void Gold_Collectible::set_pos(int x, int y) { setPosition(x, y); }

int Gold_Collectible::get_pos(char z) { return (z == 'x') ? getPosition().x : getPosition().y; }

void Gold_Collectible::draw(sf::RenderWindow& window) { window.draw(*this); }