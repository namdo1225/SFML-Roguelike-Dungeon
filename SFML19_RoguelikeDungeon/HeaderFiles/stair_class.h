/**
*
* File: stair_class.h
* Description: Contain the declaration of the Stair class, which represents a stair which player could
* use to get to the next floor.
*
*/

#ifndef STAIR_H
#define STAIR_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Stair : public sf::RectangleShape {
public:

	static sf::Texture stair_tex;

	/**
	* Constructor for Stair.
	*/
	Stair();

	/**
	* Constructor for Stair.
	*
	* Parameter:
	*	x: an int for the stair's x position.
	* 	y: an int for the stair's x position.
	*/
	Stair(int x, int y);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

#endif