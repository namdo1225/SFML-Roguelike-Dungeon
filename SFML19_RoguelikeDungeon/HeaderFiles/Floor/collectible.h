/**
*
* File: collectible.h
* Description: Contain the declaration of the Collectible class, which represents a collectible which player could
* collect into their inventory.
*
*/

#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "Floor/floor_object.h"

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
	*	x: a float for the item's x position.
	* 	y: a float for the item's x position.
	*	id: an unsigned int for the item's id.
	*/
	Collectible(float x, float y, unsigned int id);

	/**
	* Getter for item's id.
	*
	* Return:
	*	an unsigned int for the item's id.
	*/
	unsigned int get_id();
};


#endif