/**
*
* File: gold_collectible.h
* Description: Contain the declaration of the Gold_Collectible class, which represents a gold piece which player could
* gather.
*
*/

#ifndef GOLD_COLLECTIBLE_H
#define GOLD_COLLECTIBLE_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Gold_Collectible : sf::RectangleShape {
private:
	unsigned int amount{ 0 };

public:

	/**
	* Constructor for Gold_Collectible.
	*/
	Gold_Collectible();

	/**
	* Constructor for Gold_Collectible.
	*
	* Parameter:
	* 	floor: an unsigned int for the current floor.
	* 	t_amount: an unsigned int for the gold amount.
	*	x: an int for the gold's x position.
	* 	y: an int for the gold's x position.
	*/
	Gold_Collectible(unsigned int floor, unsigned int t_amount, int x, int y);

	/**
	* Getter for the amount.
	*
	* Return:
	*	an unsigned int for the gold amount.
	*/
	unsigned int get_amount();

	/**
	* Setter for gold's position.
	*
	* Parameter:
	*	x: an int for the gold's new x position.
	*	y: an int for the gold's new y position.
	*/
	void set_pos(int x, int y);

	/**
	* Getter for gold's position.
	*
	* Parameter:
	*	z: a char. 'x' to get gold's x position. 'y' to get gold's y position.
	*
	* Return:
	*	an int for the gold's x or y position.
	*/
	int get_pos(char z);

	/**
	* Draw the gold on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the gold will be drawn on.
	*/
	void draw(sf::RenderWindow& window);
};

#endif