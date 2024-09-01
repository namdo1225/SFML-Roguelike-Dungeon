#include "Floor/floor_object.h"

#ifndef SHOP_H
#define SHOP_H

/**
* Represents a shop on the map.
*/
class Shop : public Floor_Object {
public:

	/**
	* Constructor for Shop.
	*/
	Shop();

	/**
	* Constructor for Shop.
	*
	* Parameter:
	*	x: the shop's x position.
	* 	y: the shop's y position.
	*/
	Shop(float x, float y);
};

#endif