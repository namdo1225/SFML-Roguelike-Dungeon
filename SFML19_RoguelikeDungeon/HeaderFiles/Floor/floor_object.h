/**
*
* File: floor_object.h
* Description: Contain the Floor_Object class which represents an object that can be placed
* on the maze's floor.
*
*/

#ifndef FLOOR_OBJECT_H
#define FLOOR_OBJECT_H

#include "..\SFML-2.6.1/include/SFML/Graphics.hpp"

class Floor_Object : public sf::RectangleShape {
public:
	Floor_Object();

	Floor_Object(const sf::Texture& texture);

	Floor_Object(float x, float y, const sf::Texture& texture);

	bool contains(float x, float y);

	bool intersects(const sf::FloatRect& rect);
};

#endif