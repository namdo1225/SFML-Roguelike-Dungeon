/**
*
* File: interactible.h
* Description: This class abstracts an interactible, an object that can be interacted with in the
* overworld of the game.
*
*/

#ifndef INTERACTIBLE_H
#define INTERACTIBLE_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "player.h"
#include "Floor/floor_object.h"

class Interactible : public Floor_Object {
public:
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
	*/
	Interactible(float x, float y);
};


#endif