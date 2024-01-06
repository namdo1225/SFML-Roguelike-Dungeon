/**
*
* File: shop_class.h
* Description: Contain the declaration of the Shop class, which represents a shop which player could
* use to buy/sell items and spells.
*
*/

#ifndef SHOP_H
#define SHOP_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Shop : public sf::RectangleShape {
public:
	static sf::Texture shop_tex;

	/**
	* Constructor for Shop.
	*/
	Shop();

	/**
	* Constructor for Shop.
	*
	* Parameter:
	*	x: an int for the shop's x position.
	* 	y: an int for the shop's x position.
	*/
	Shop(int x, int y);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

#endif