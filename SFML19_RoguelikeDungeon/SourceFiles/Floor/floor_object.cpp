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

Floor_Object::Floor_Object() {
	setSize(sf::Vector2f(40, 40));
	setTextureRect(sf::IntRect(0, 0, 40, 40));
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
