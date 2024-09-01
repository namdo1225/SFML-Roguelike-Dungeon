#ifndef FLOOR_H
#define FLOOR_H

#include "collectible.h"
#include "gold_collectible.h"
#include "interactible.h"
#include "map.h"
#include "room.h"
#include "shop.h"
#include "stair.h"
#include <SFML/Graphics/Rect.hpp>
#include <vector>

/**
* Represents a dungeon floor.
*/
class Floor {
private:
	bool shopExist{ false };
	Shop shop;
	Stair stair;

	/**
	* Constructs rooms and doors for a new floor.
	*/
	void createRoomDoor();

public:
	Map map;

	std::vector<Room> rooms;
	std::vector<Collectible> collectibles;
	std::vector<Gold_Collectible> golds;
	std::vector<Interactible> interactibles;

	/**
	* Constructor for Floor.
	* 
	* Parameter:
	*	load: true if the floor is being loaded from a save file.
	*/
	Floor(bool load = false);

	/**
	* Helper method to set a room's position && size.
	*
	* Parameter:
	*	rand_door: the rotation of the door.
	*	rand_coord: x coordinate of the room.
	*	rand_size: x size of the room.
	*	rand_coord_2:  y coordinate of the room.
	*	rand_size_2: y size of the room.
	*	rand_room_coord: x position of another room.
	*	rand_room_size: x size position of another room.
	*	rand_room_coord_2: y position of another room.
	*	rand_room_size_2:y size of another room.
	*	conflict_room: true if the room being made has a conflict with another room.
	*	k: for indexing purpose.
	*/
	void setRmPosSize(int rand_door, int& rand_coord, int& rand_size, int& rand_coord_2, int& rand_size_2, int rand_room_coord,
		int rand_room_size, int rand_room_coord_2, int rand_room_size_2, bool& conflict_room, int& k);

	/**
	* Helper method to set door's position (can be either x or y).
	* 
	* Parameter:
	*	rand_door_coord: the coordinate that will be set as the door's coordinate (can be either x or y).
	*	rand_coord: the position of rand_door_coord.
	*	rand_size: the size of rand_door_coord.
	*/
	void setDoorPos1(int& rand_door_coord, int rand_coord, int rand_size);

	/**
	* Helper method to set door's position (can be either x || y).
	* 
	* Parameter:
	*	rand_door_coord:the coordinate that will be set as the door's coordinate (can be either x or y).
	*	rand_coord: the position of rand_door_coord.
	*	rand_size:the size of rand_door_coord.
	*	rand_room_coord: the room's position. Useful to calculate door's position.
	*	rand_room_size: the room's size. Useful to calculate door's position.
	*/
	void setDoorPos2(int& rand_door_coord, int rand_coord, int rand_size, int rand_room_coord, int rand_room_size);

	/**
	* Make the stair for the current floor.
	*/
	void createStair();

	/**
	* Make the shop for the current floor.
	*/
	void createShop();

	/**
	* Getter for shopExist.
	* 
	* Return:
	*	true if shop exists.
	*/
	bool isShopExist();

	/**
	* Setter for shop's position.
	*
	* Parameter:
	*	x: shop's new x position.
	* 	y: shop's new y position.
	*/
	void setShopPos(int x, int y);

	/**
	* Setter for stair's position.
	*
	* Parameter:
	*	x: stair's new x position.
	* 	y: stair's new y position.
	*/
	void setStairPos(int x, int y);

	/**
	* Getter for shop's position.
	*
	* Parameter:
	*	z: 'x' for the shop's x. 'y' for the shop's y.
	*
	* Return:
	*	shop's x or y position.
	*/
	int getShopPos(char z);

	/**
	* Getter for stair's position.
	*
	* Parameter:
	*	z: 'x' for the stair's x. 'y' for the stair's y.
	*
	* Return:
	*	the stair's x or y position.
	*/
	int getStairPos(char z);

	/**
	* Draw the golds, collectibles, rooms, doors, etc. on the game's window.
	*/
	void draw();

	/**
	* Load/Create room for the current floor from existing data.
	*
	* Parameter:
	*	x: the room's x position.
	*	y: the room's y position.
	*	sx: the room's width.
	*	sy: the room's height.
	*/
	void loadRoom(int x, int y, int sx, int sy);

	/**
	* Load door for the current floor from existing data.
	*
	* Parameter:
	*	x: the door's x position.
	*	y: the door's y position.
	*	rot: the door's rotation.
	*	slot0: the door's filled slot 0.
	*	slot1: the door's filled slot 1.
	*	slot2: the door's filled slot 2.
	*	slot3: the door's filled slot 3.
	*/
	void loadDoor(int x, int y, int rot, int slot0, int slot1, int slot2, int slot3);

	/**
	* Load stair for the current floor from existing data.
	*
	* Parameter:
	*	x: the stair's x position.
	*	y: the stair's y position.
	*/
	void loadStair(int x, int y);

	/**
	* Load item for the current floor from existing data.
	*
	* Parameter:
	*	x: the item's x position.
	*	y: the item's y position.
	*	id: the item's id.
	*/
	void loadCollectible(int x, int y, unsigned int id);

	/**
	* Load gold collectible for the current floor from existing data.
	*
	* Parameter:
	*	x: the gold's x position.
	*	y: the gold's y position.
	*	amount: the gold's amount.
	*/
	void loadGold(int x, int y, unsigned int amount);

	/**
	* Load shop for the current floor from existing data.
	*
	* Parameter:
	*	x: the shop's x position.
	*	y: the shop's y position.
	*/
	void loadShop(int x, int y);

	/**
	* Load interactible for the current floor from existing data.
	*
	* Parameter:
	*	x: the interactible's x position.
	*	y: the interactible's y position.
	*	hidden: True if interactible is hidden from player.
	*/
	void loadInteractible(int x, int y, bool hidden);

	/**
	* Make item collectibles for the current floor.
	*
	* Parameter:
	*	floor: the current floor number.
	*/
	void makeCollectible(unsigned int floor);

	/**
	* Make gold collectibles for the current floor.
	*
	* Parameter:
	*	floor: the current floor number.
	*/
	void makeGold(unsigned int floor);

	/**
	* Make interactibles for the current floor.
	*
	* Parameter:
	*	floor: the current floor number.
	*/
	void makeInteractible(unsigned int floor);

	/**
	* Make the map for the current floor.
	*/
	void makeMap();

	/**
	* Checks if the stair intersect with a rectangle.
	* 
	* Parameter:
	*	rect: The rectangle for interseciton check.
	* 
	* Return:
	*	true if an intersection exists.
	*/
	bool intersectStair(const sf::FloatRect& rect);

	/**
	* Checks if the shop intersect with a rectangle.
	* 
	* Parameter:
	*	rect: The rectangle for interseciton check.
	* 
	* Return:
	*	true if an intersection exists.
	*/
	bool intersectShop(const sf::FloatRect& rect);
};

#endif