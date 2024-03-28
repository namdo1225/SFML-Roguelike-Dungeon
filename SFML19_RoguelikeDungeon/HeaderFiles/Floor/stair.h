/**
*
* File: stair.h
* Description: Contain the declaration of the Stair class, which represents a stair which player could
* use to get to the next floor.
*
*/

#ifndef STAIR_H
#define STAIR_H

#include "Floor/floor_object.h"

class Stair : public Floor_Object {
public:
	/**
	* Constructor for Stair.
	*/
	Stair();

	/**
	* Constructor for Stair.
	*
	* Parameter:
	*	x: a float for the stair's x position.
	* 	y: a float for the stair's x position.
	*/
	Stair(int x, int y);
};

#endif