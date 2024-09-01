#include "room.h"
#include <vector>

#ifndef MAP_H
#define MAP_H

/**
* Represents a map which player could
* use to view the current floor's layout.
*/
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
	* 
	* Parameter:
	*	rooms: list of rooms for the map to be created
	*/
	Map(std::vector<Room> &rooms);

	/**
	* Draw the map on the game's window.
	*/
	void draw();
};

#endif