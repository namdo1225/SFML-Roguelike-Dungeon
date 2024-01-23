/**
*
* File: interactibles.h
* Description: This class abstracts an interactible, an object that can be interacted with in the
* overworld of the game.
*
*/

#ifndef INTERACTIBLE_H
#define INTERACTIBLE_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "player.h"

class Interactible : sf::RectangleShape {
private:
	unsigned int amount{ 0 };

public:

	/**
	* Constructor for Interactible.
	*
	*/
	Interactible();

	/**
	* Constructor for Interactible.
	*
	* Parameter:
	*	x: an int for the interactible's x position.
	* 	y: an int for the interactible's x position.
	*/
	Interactible(int x, int y);

	/**
	* Setter for interactible's position.
	*
	* Parameter:
	*	x: an int for the interactible's new x position.
	*	y: an int for the interactible's new y position.
	*/
	void set_pos(int x, int y);

	/**
	* Getter for interactible's position.
	*
	* Parameter:
	*	z: a char. 'x' to get interactible's x position. 'y' to get interactible's y position.
	*
	* Return:
	*	an int for the interactible's x or y position.
	*/
	int get_pos(char z);

	/**
	* Draw the interactible on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the interactible will be drawn on.
	*/
	void draw(sf::RenderWindow& window);
};


#endif