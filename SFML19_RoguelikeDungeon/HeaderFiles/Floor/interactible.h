#include "Floor/floor_object.h"

#ifndef INTERACTIBLE_H
#define INTERACTIBLE_H

/**
* An object that can be interacted with in the
* overworld of the game.
*/
class Interactible : public Floor_Object {
private:
	bool hidden = false;

public:
	bool inactive = false;

	/**
	* Constructor for Interactible.
	*/
	Interactible();

	/**
	* Constructor for Interactible.
	*
	* Parameter:
	*	x: a float for the interactible's x position.
	* 	y: a float for the interactible's x position.
	*	hidden: a bool. Whether the interactible is hidden from player.
	*/
	Interactible(float x, float y, bool hidden);

	/**
	* Sets an interactible to inactive.
	*/
	void setInactive();

	/**
	* Getter for hidden.
	* 
	* Return:
	*	true if interactible is hidden.
	*/
	bool getHidden();
};


#endif