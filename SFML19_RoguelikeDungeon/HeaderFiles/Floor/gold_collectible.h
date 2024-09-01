#include "Floor/floor_object.h"

#ifndef GOLD_COLLECTIBLE_H
#define GOLD_COLLECTIBLE_H

/**
* Represents a gold piece which player could gather.
*/
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
	* 	floor: the current floor.
	* 	amount: the gold amount.
	*	x: the gold's x position.
	* 	y: the gold's x position.
	*/
	Gold_Collectible(unsigned int floor, unsigned int amount, float x, float y);

	/**
	* Getter for the amount.
	*
	* Return:
	*	the gold amount.
	*/
	unsigned int getGold();
};

#endif