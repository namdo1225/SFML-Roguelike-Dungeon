/**
*
* File: map_class.h
* Description: Contain the declaration of the Map class, which represents a map which player could
* use to view the current floor's layout.
*
*/

#ifndef MAP_H
#define MAP_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "room_class.h"
#include <array>

class Map {
private:
	std::vector<Room> map;

	static std::array<sf::Text, 4> directions;
	static std::array<sf::Text, 2> zooms;

	static int factor;

public:
	static void setup(sf::Font& font);

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
	void draw(sf::RenderWindow& window);

	/**
	* Move the map.
	*
	* Parameter:
	*	direction: a char. 'u' to move map up. 'r' to move right. 'd' to move down. 'l' to move left.
	*/
	void move(char direction);
	
	/**
	* Zoom in/out on the map.
	*
	* Parameter:
	*	direction: a char. 'i' to zoom in on the map. 'o' to zoom out.
	*/
	void zoom(char direction);

	/**
	* Handles map movement event for the map.
	*
	* Parameter:
	*	x: an int for mouse's x coordinate input.
	*	y: an int for mouse's x coordinate input.
	*/
	void handleEvent(int x, int y);
};

#endif