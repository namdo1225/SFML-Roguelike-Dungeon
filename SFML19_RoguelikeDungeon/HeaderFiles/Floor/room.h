#ifndef ROOM_H
#define ROOM_H

#include "Floor/door.h"
#include "Floor/floor_object.h"
#include <array>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

/**
* Enum to represent object in room.
*/
enum RoomObject { PlayerObject = -3, ObstacleObject, NoObject };

/**
* Represents a room which player could be in.
*/
class Room : public Floor_Object {
private:
	bool visited{ false };

	unsigned int unscaledWidth = 0, unscaledHeight = 0;

	std::array<std::vector<Door>, 4> doors = {std::vector<Door>(), std::vector<Door>(), std::vector<Door>(), std::vector<Door>() };

	std::array<std::vector<Room*>, 4> connectedRooms = { std::vector<Room*>(), std::vector<Room*>(), std::vector<Room*>(), std::vector<Room*>() };

public:
	const static unsigned int MAX_ROOMS = 50;
	const static unsigned int MAX_WIDTH = 35;
	const static unsigned int MAX_HEIGHT = 35;

	/**
	* Constructor for Room.
	* 
	* Parameter:
	*	x: the room's x position.
	*	y: the room's y position.
	*	unscaledW: the room's unscaled width.
	*	unscaledH: the room's unscaled height.
	*/
	Room(float x = -1, float y = -1, int unscaledW = -1, int unscaledH = -1);

	/**
	* Setter for room's position & size.
	*
	* Parameter:
	*	x: the room's new x position.
	*	y: the room's new y position.
	*	w: the room's new width.
	*	h: the room's new height.
	*/
	void setPosSize(int x, int y, int w, int h);

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
	* Get door slot for the room.
	*
	* Parameter:
	*	i: index the door's slot.
	*
	* Return:
	*	false = no door (slot available), true = door.
	*/
	std::array<std::vector<Door>, 4> getDoors();

	/**
	* Draw the room on the game's window.
	*
	* Parameter:
	*	door: true if draw door. false if draw room.
	*/
	void draw(bool drawDoor = false);

	/**
	* Whether a player has visited the room or not.
	*
	* Return:
	*	true if room is visited.
	*/
	bool getVisited();

	/**
	* Sets a room as visited by player.
	*/
	void setVisisted();

	/**
	* Add adjacent room for current room and connect by door.
	* 
	* Parameter:
	*	door: The door to connect the rooms.
	*	room: The connecting room.
	*/
	void addAdjacentRoom(Door door, Room* room);

	/**
	* Check if 2 entities are in door range.
	* 
	* Parameter:
	*	otherRoom: the room that the opposing entity is in.
	*	entity1: The rectangle of the first entity.
	*	entity2: The rectangle of the second entity.
	* 
	* Return:
	*	Whether entities are in range.
	*/
	bool areEntitiesInDoorRange(Room* otherRoom, const sf::FloatRect& entity1, const sf::FloatRect& entity2);

	/**
	* Check if entity moving in a certain direction if it touches a door in that direction.
	* 
	* Parameter:
	*	direction: Direction of movement.
	*	entity: Entity touching the door.
	* 
	* Return:
	*	Whether entity touch door.
	*/
	bool touchDoor(Direction direction, const sf::FloatRect& entity);
};


#endif