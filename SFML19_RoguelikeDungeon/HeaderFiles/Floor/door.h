#ifndef DOOR_H
#define DOOR_H

#include "Floor/floor_object.h"

/**
* Enum to represent a door the side of a wall.
*/
enum Direction { Top, Right, Bottom, Left };

/**
* Represents a room's door.
*/
class Door : public Floor_Object {
private:
	Direction direction = Top;

	unsigned int fromRoomId = 0;

	unsigned int toRoomId = 0;

public:
	
	/**
	* Constructor for Door.
	* 
	* Parameter:
	*	x: door's x coordinate.
	*	y: door's y coordinate.
	*	size: door's size.
	*	direction: door's direction.
	*	fromRoomId: door originated from this room.
	*	toRoomId: door leads to this room.
	*	load: whether door is loaded from a save.
	*/
	Door(float x, float y, float size, Direction direction, unsigned int fromRoomId, unsigned int toRoomId, bool load = false);

	/**
	* Check if a coordinate (like projectile) can pass through a door.
	* 
	* Parameter:
	*	x: object's x coordinate.
	*	y: object's y coordinate.
	*	haveToTouch: whehter the object has to touch the door.
	* 
	* Return:
	*	whether object can pass.
	*/
	bool canPass(float x, float y, bool haveToTouch = false);

	/**
	* Getter for direction.
	* 
	* Return:
	*	direction of the door.
	*/
	Direction getDirection();
	
	/**
	* Getter for fromRoomId.
	* 
	* Return:
	*	id for room where door originated.
	*/
	unsigned int getFromRoomId();

	/**
	* Getter for toRoomId.
	* 
	* Return:
	*	id for room where door is leading.
	*/
	unsigned int getToRoomId();

};

#endif