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
	Door(float x, float y, float size, Direction direction, unsigned int fromRoomId, unsigned int toRoomId, bool load = false);

	bool canPass(float x, float y, bool haveToTouch = false);

	Direction getDirection();

	unsigned int getFromRoomId();

	unsigned int getToRoomId();

};

#endif