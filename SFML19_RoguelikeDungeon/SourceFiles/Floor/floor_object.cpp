/**
*
* File: floor_object.c
* Description: Contain the Floor_Object class implementation.
*
*/

#include "Floor/floor_object.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <Manager/sf_manager.h>

Floor_Object::Floor_Object() {
	setSize(sf::Vector2f(SF_Manager::TILE, SF_Manager::TILE));
	setTextureRect(sf::IntRect(0, 0, SF_Manager::TILE, SF_Manager::TILE));
}

Floor_Object::Floor_Object(const sf::Texture &texture) : Floor_Object() {
	setTexture(&texture, false);
}

Floor_Object::Floor_Object(float x, float y, const sf::Texture& texture) : Floor_Object(texture) {
	setPosition(x, y);
}

bool Floor_Object::contains(float x, float y) {
	return getGlobalBounds().contains(sf::Vector2f(x, y));
}

bool Floor_Object::intersects(const sf::FloatRect& rect) {
	return getGlobalBounds().intersects(rect);
}

void Floor_Object::draw() {
	SF_Manager::window.draw(*this);
}

sf::FloatRect Floor_Object::getRect() {
	return getGlobalBounds();
}

void Floor_Object::setPosition(float x, float y) {
	sf::RectangleShape::setPosition(x, y);
}