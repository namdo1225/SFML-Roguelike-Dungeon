/**
*
* File: shop.h
* Description: Contain the declaration of the Shop class, which represents a shop which player could
* use to buy/sell items and spells.
*
*/

#ifndef SHOP_H
#define SHOP_H

#include "Floor/floor_object.h"

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
	*	x: a float for the shop's x position.
	* 	y: a float for the shop's x position.
	*/
	Shop(float x, float y);
};

#endif