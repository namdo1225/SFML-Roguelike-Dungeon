/**
*
* File: room.h
* Description: Contain the declaration of the Room class, which represents a room which player could
* be in.
*
*/

#ifndef ROOM_H
#define ROOM_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include <array>

class Room {
private:
	// 0, 2 = horizontal. 1, 3 = vertical
	unsigned int door_rotation;

	bool door{ false };

	// false = no door (slot available), true = door.
	// 0 = north, horizontal, 2 = south, horizontal. 1 = west, vertical, 3 = east, vertical.
	std::array<bool, 4> doors{ false, false, false, false };

	sf::RectangleShape dr_draw, rm_draw;

public:

	/**
	* Constructor for Room.
	*/
	Room();

	/**
	* Setter for room's position && size.
	*
	* Parameter:
	*	x: an int for the room's new x position.
	*	y: an int for the room's new y position.
	*	sx: an int for the room's new width.
	*	sy: an int for the room's new height.
	*/
	void set_pos_and_size(int x, int y, int sx, int sy);

	/**
	* Setter for door's position, size, and rotation.
	*
	* Parameter:
	*	x: an int for the door's new x position or width.
	*	y: an int for the door's new y position or height.
	*	rotation: an int. 0, 2 = horizontal door rotation. 1, 3 = vertical door rotation.
	*/
	void set_door(int x, int y, int rotation);

	/**
	* Getter for information about the room.
	*
	* Parameter:
	*	z: a char. 'x' to get room's x position. 'y' to get room's y position.
	*		'w' for width. 'h' for height. '1' for x + width. '2' for y + height.
	*		'3' for the y coordinate of the opposite corner of the room's original position.
	*
	* Return:
	*	an int for the room's position or other useful info.
	*/
	int get_rm(char z);

	/**
	* Getter for information about the door.
	*
	* Parameter:
	*	z: a char. 'x' to get door's x position. 'y' to get door's y position.
	*		'w' for width. 'h' for height. '0' - '3' for doors' filled slot.
	* 
	* Return:
	*	an float for the door's position or other useful info.
	*/
	int get_door(char z);

	/**
	* Check whether two pairs of coordinate is in the room (representing a square).
	*
	* Parameter:
	*	x: an int for the first x coordinate.
	*	y: an int for the first y coordinate.
	*	x2: an int for the second x coordinate.
	*	y2: an int for the second y coordinate.
	*
	* Return:
	*	a bool. true if coordinates are in the room. false if not.
	*/
	bool in_room(int x, int y, int x2, int y2);

	/**
	* Check whether a door exist for the room.
	*
	* Return:
	*	a bool. true if a door exist for the room.
	*/
	bool door_exist();

	/**
	* Check whether two pairs of coordinate is touching a door.
	*
	* Parameter:
	*	x: an int for the first x coordinate.
	*	y: an int for the first y coordinate.
	*	x2: an int for the second x coordinate.
	*	y2: an int for the second y coordinate.
	*
	* Return:
	*	a bool. true if the pairs are touching the door. false if not.
	*/
	bool touch_door(int x, int y, int x2, int y2);

	/**
	* Get door slot for the room.
	*
	* Parameter:
	*	i: an int to index the slot position.
	*
	* Return:
	*	a bool. false = no door (slot available), true = door.
	*/
	bool get_doors(unsigned int i);

	/**
	* Set door slot for the room.
	*
	* Parameter:
	*	i: an int to index the slot position.
	*	j: a bool for the value of the slot position.
	*/
	void set_doors(unsigned int i, bool j);

	/**
	* Draw the room on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the room will be drawn on.
	*	d: a char. 'r' to draw room. 'd' to draw door.
	*/
	void draw(sf::RenderWindow& window, char d);
};


#endif