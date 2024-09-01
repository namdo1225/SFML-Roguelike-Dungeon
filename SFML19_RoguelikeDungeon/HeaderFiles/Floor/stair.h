#include "Floor/floor_object.h"

#ifndef STAIR_H
#define STAIR_H

/**
* Represents a stair which player could use to get to the next floor.
*/
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
	*	x: the stair's x position.
	* 	y: the stair's y position.
	*/
	Stair(float x, float y);
};

#endif