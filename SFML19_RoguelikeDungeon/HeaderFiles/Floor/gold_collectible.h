/**
*
* File: gold_collectible.h
* Description: Contain the declaration of the Gold_Collectible class, which represents a gold piece which player could
* gather.
*
*/

#ifndef GOLD_COLLECTIBLE_H
#define GOLD_COLLECTIBLE_H

#include "Floor/floor_object.h"

class Gold_Collectible : public Floor_Object {
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
	* 	amount: an unsigned int for the gold amount.
	*	x: an int for the gold's x position.
	* 	y: an int for the gold's x position.
	*/
	Gold_Collectible(unsigned int floor, unsigned int amount, float x, float y);

	/**
	* Getter for the amount.
	*
	* Return:
	*	an unsigned int for the gold amount.
	*/
	unsigned int get_amount();
};

#endif