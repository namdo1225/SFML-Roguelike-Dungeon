#ifndef FLOOR_OBJECT_H
#define FLOOR_OBJECT_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
* Represents an object that can be placed
* on the maze's floor.
*/
class Floor_Object : public sf::RectangleShape {
public:

	/**
	* Constructor for Floor_Object.
	*/
	Floor_Object();

	/**
	* Constructor for Floor_Object and applies texture.
	* 
	* Parameter:
	*	texture: the texture to apply for the object.
	*/
	Floor_Object(const sf::Texture& texture);

	/**
	* Constructor for Floor_Object, applies texture, and set position.
	*
	* Parameter:
	*	x: object's x position.
	*	y: object's y position.
	*	texture: the texture to apply for the object.
	*/
	Floor_Object(float x, float y, const sf::Texture& texture);

	/**
	* Checks if a coordinate is contained by the object.
	* 
	* Parameter:
	*	x: x coordinate.
	*	y: y coordinate.
	* 
	* Return:
	*	true if coordinate is contained.
	*/
	bool contains(float x, float y);

	/**
	* Checks if a rectangle intersects with object.
	*
	* Parameter:
	*	rect: the rect to check for intersection
	*
	* Return:
	*	true if intersection exists.
	*/
	bool intersects(const sf::FloatRect& rect);
};

#endif