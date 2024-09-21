#ifndef ROOM_H
#define ROOM_H

#include "Floor/floor_object.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <Shape/full_rectangle.h>

/**
* Enum to represent a door the side of a wall.
*/
enum Door { Top, Right, Bottom, Left };

/**
* Represents a room which player could be in.
*/
class Room : private Floor_Object {
private:
	Door doorRotation = Top;

	bool door{ false }, visited{ false };

	/**
	* false = no door / (slot available)
	*/
	bool doors[4] = { false, false, false, false };

	Full_Rectangle doorRect = Full_Rectangle(-100.f, -100.f, 0.f, 0.f, false, true,
		sf::Color::Transparent, sf::Color::White);

public:

	/**
	* Constructor for Room.
	*/
	Room();

	/**
	* Setter for room's position & size.
	*
	* Parameter:
	*	x: the room's new x position.
	*	y: the room's new y position.
	*	sx: the room's new width.
	*	sy: the room's new height.
	*/
	void setPosSize(int x, int y, int sx, int sy);

	/**
	* Setter for door's position, size, and rotation.
	*
	* Parameter:
	*	x: the door's new x position or width.
	*	y: the door's new y position or height.
	*	rotation: 0, 2 = horizontal door rotation. 1, 3 = vertical door rotation.
	*/
	void setDoor(int x, int y, int rotation);

	/**
	* Getter for info about the room.
	*
	* Parameter:
	*	z: 'x' to get room's x. 'y' to get room's y. 'w' for width. 'h' for height.
	*		'1' for x + width. '2' for y + height.
	*		'3' for the y of the opposite corner of the room.
	*
	* Return:
	*	room's position or other useful info.
	*/
	int getRoom(char z);

	/**
	* Getter for info about the door.
	*
	* Parameter:
	*	z: 'x' to get door's x. 'y' to get door's y.
	*		'w' for width. 'h' for height. '0' - '3' for doors' filled slot.
	* 
	* Return:
	*	the door's position or other useful info.
	*/
	int getDoor(char z);

	/**
	* Check whether two pairs of coordinate is in the room (representing a square).
	*
	* Parameter:
	*	x: the first x coordinate.
	*	y: the first y coordinate.
	*	x2: the second x coordinate.
	*	y2: the second y coordinate.
	*
	* Return:
	*	true if coordinates are in the room.
	*/
	bool inRoom(int x, int y, int x2, int y2);

	/**
	* Check whether a door exist for the room.
	*
	* Return:
	*	true if a door exist for the room.
	*/
	bool existDoor();

	/**
	* Check whether 2 pairs of coordinate is touching a door.
	*
	* Parameter:
	*	x: the first x coordinate.
	*	y: the first y coordinate.
	*	x2: the second x coordinate.
	*	y2: the second y coordinate.
	*
	* Return:
	*	true if the pairs are touching the door.
	*/
	bool touchDoor(int x, int y, int x2, int y2);

	/**
	* Get door slot for the room.
	*
	* Parameter:
	*	i: index the door's slot.
	*
	* Return:
	*	false = no door (slot available), true = door.
	*/
	bool getDoors(unsigned int i);

	/**
	* Set door slot for the room.
	*
	* Parameter:
	*	i: index the door slot position.
	*	j: the value of the door slot position.
	*/
	void setDoors(unsigned int i, bool j);

	/**
	* Draw the room on the game's window.
	*
	* Parameter:
	*	door: true if draw door. false if draw room.
	*/
	void draw(bool door = false);

	/**
	* Whether a rectangle intersect with the room.
	* 
	* Parameter:
	*	rect: The rectangle to check for intersection.
	* 
	* Return:
	*	true if intersection exist.
	*/
	bool intersects(const sf::FloatRect& rect);

	/**
	* Whether a player has visited the room or not.
	*
	* Return:
	*	true if room is visited.
	*/
	bool getVisited();

	void setVisisted();
};


#endif