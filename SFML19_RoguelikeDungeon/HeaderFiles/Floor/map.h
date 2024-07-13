/**
*
* File: map.h
* Description: Contain the declaration of the Map class, which represents a map which player could
* use to view the current floor's layout.
*
*/

#ifndef MAP_H
#define MAP_H

#include "..\SFML-2.6.1/include/SFML/Graphics.hpp"
#include "room.h"
#include <array>

class Map {
private:
	std::vector<Room> map;
public:
	/**
	* Constructor for Map.
	*/
	Map();

	/**
	* Constructor for Map.
	*/
	Map(std::vector<Room> &rooms);

	/**
	* Draw the map on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the map will be drawn on.
	*/
	void draw();
};

#endif