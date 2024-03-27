/**
*
* File: stair.h
* Description: Contain the declaration of the Stair class, which represents a stair which player could
* use to get to the next floor.
*
*/

#ifndef STAIR_H
#define STAIR_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Stair : public sf::RectangleShape {
public:
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
};

#endif