/**
*
* File: collectible.h
* Description: Contain the declaration of the Collectible class, which represents a collectible which player could
* collect into their inventory.
*
*/

#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Collectible : sf::RectangleShape {
private:
	unsigned int id{ 0 };

public:
	/**
	* Constructor for Collectible.
	*/
	Collectible();

	/**
	* Constructor for Collectible.
	*
	* Parameter:
	*	x: an int for the item's x position.
	* 	y: an int for the item's x position.
	*	t_id: an unsigned int for the item's id.
	*/
	Collectible(int x, int y, unsigned int t_id);

	/**
	* Getter for item's position.
	*
	* Parameter:
	*	z: a char. 'x' to get item's x position. 'y' to get item's y position.
	*
	* Return:
	*	an int for the item's x or y position.
	*/
	int get_pos(char z);

	/**
	* Getter for item's id.
	*
	* Return:
	*	an unsigned int for the item's id.
	*/
	unsigned int get_id();

	/**
	* Setter for item's position.
	*
	* Parameter:
	*	x: an int for the item's new x position.
	*	y: an int for the item's new y position.
	*/
	void set_pos(int x, int y);

	/**
	* Draw the item on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the item will be drawn on.
	*/
	void draw(sf::RenderWindow& window);
};


#endif