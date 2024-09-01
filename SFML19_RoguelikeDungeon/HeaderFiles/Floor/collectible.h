#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "Floor/floor_object.h"

/**
* Represents a collectible which player could
* collect into their inventory.
*/
class Collectible : public Floor_Object {
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
	*	x: Item's x position.
	* 	y: Item's x position.
	*	id: Item's id.
	*/
	Collectible(float x, float y, unsigned int id);

	/**
	* Getter for item's id.
	*
	* Return:
	*	Item's id.
	*/
	unsigned int getID();
};


#endif