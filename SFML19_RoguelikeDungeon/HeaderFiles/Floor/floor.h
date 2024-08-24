/**
*
* File: floor.h
* Description: Contain Floor class, which represents a dungeon floor.
*
*/

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

class Floor {
private:
	bool fl_shop{ false };
	Shop shop;
	Stair stair;

	/**
	* Constructs rooms and doors for a new floor.
	*/
	void make_room_door();

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
	*	load: a bool for whether the floor is being loaded from a save file.
	*/
	Floor(bool load = false);

	/**
	* Helper method to set a room's position && size.
	*
	* Parameter:
	*	rand_door: an int for the rotation of the door.
	*	rand_coord: an int&. The x coordinate of the room.
	*	rand_size: an int&. The x size of the room.
	*	rand_coord_2: an int&. The y coordinate of the room.
	*	rand_size_2: an int&. The y size of the room.
	*	rand_room_coord: an int. The x position of another room.
	*	rand_room_size: an int. The x size position of another room.
	*	rand_room_coord_2: an int. The y position of another room.
	*	rand_room_size_2: an int. The y size of another room.
	*	conflict_room: a bool&. Indicate whether the room being made has a conflict with another room.
	*	k: an int&.
	*/
	void set_rm_pos_size(int rand_door, int& rand_coord, int& rand_size, int& rand_coord_2, int& rand_size_2, int rand_room_coord,
		int rand_room_size, int rand_room_coord_2, int rand_room_size_2, bool& conflict_room, int& k);

	/**
	* Helper method to set door's position (can be either x || y).
	* 
	* Parameter:
	*	rand_door_coord: an int& for the coordinate that will be set as the door's coordinate (can be either x or y).
	*	rand_coord: an int for the position of rand_door_coord.
	*	rand_size: an int for the size of rand_door_coord.
	*/
	void set_door_pos_1(int& rand_door_coord, int rand_coord, int rand_size);

	/**
	* Helper method to set door's position (can be either x || y).
	* 
	* Parameter:
	*	rand_door_coord: an int& for the coordinate that will be set as the door's coordinate (can be either x or y).
	*	rand_coord: an int for the position of rand_door_coord.
	*	rand_size: an int for the size of rand_door_coord.
	*	rand_room_coord: an int for the room's position. Useful to calculate door's position.
	*	rand_room_size: an int for the room's size. Useful to calculate door's position.
	*/
	void set_door_pos_2(int& rand_door_coord, int rand_coord, int rand_size, int rand_room_coord, int rand_room_size);

	/**
	* Make the stair for the current floor.
	*/
	void make_stair();

	/**
	* Make the shop for the current floor.
	*/
	void make_shop();

	/**
	* Check if shop exist.
	* 
	* Return:
	*	a bool. true if shop exists. false if not.
	*/
	bool shop_exist();

	/**
	* Setter for shop's position.
	*
	* Parameter:
	*	x: an int for shop's new x position.
	* 	y: an int for shop's new y position.
	*/
	void set_shop_pos(int x, int y);

	/**
	* Setter for stair's position.
	*
	* Parameter:
	*	x: an int for stair's new x position.
	* 	y: an int for stair's new y position.
	*/
	void set_stair_pos(int x, int y);

	/**
	* Getter for shop's position.
	*
	* Parameter:
	*	z: a char. 'x' for the shop's x position. 'y' for the shop's y position.
	*
	* Return:
	*	an int for the shop's x || y position.
	*/
	int get_shop_pos(char z);

	/**
	* Getter for stair's position.
	*
	* Parameter:
	*	z: a char. 'x' for the stair's x position. 'y' for the stair's y position.
	*
	* Return:
	*	an int for the stair's x || y position.
	*/
	int get_stair_pos(char z);

	/**
	* Draw the golds, collectibles, rooms, && doors on the game's window.
	*/
	void draw();

	/**
	* Load/Create room for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the room's x position.
	*	y: an int for the room's y position.
	*	sx: an int for the room's width.
	*	sy: an int for the room's height.
	*/
	void load_room(int x, int y, int sx, int sy);

	/**
	* Load/Create door for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the door's x position.
	*	y: an int for the door's y position.
	*	rot: an int for the door's rotation.
	*	slot0: an int for the door's filled slot 0.
	*	slot1: an int for the door's filled slot 1.
	*	slot2: an int for the door's filled slot 2.
	*	slot3: an int for the door's filled slot 3.
	*/
	void load_door(int x, int y, int rot, int slot0, int slot1, int slot2, int slot3);

	/**
	* Load/Create stair for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the stair's x position.
	*	y: an int for the stair's y position.
	*/
	void load_stair(int x, int y);

	/**
	* Load/Create item for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the item's x position.
	*	y: an int for the item's y position.
	*	id: an unsigned int for the item's id.
	*/
	void load_collectible(int x, int y, unsigned int id);

	/**
	* Load/Create gold collectible for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the gold's x position.
	*	y: an int for the gold's y position.
	*	amount: an unsigned int for the gold's amount.
	*/
	void load_gold(int x, int y, unsigned int amount);

	/**
	* Load/Create shop for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the shop's x position.
	*	y: an int for the shop's y position.
	*/
	void load_shop(int x, int y);

	/**
	* Load/Create interactible for the current floor from existing data.
	*
	* Parameter:
	*	x: an int for the interactible's x position.
	*	y: an int for the interactible's y position.
	*	hidden: a bool. Whether the interactible is hidden from player.
	*/
	void load_interactible(int x, int y, bool hidden);

	/**
	* Make item collectibles for the current floor.
	*
	* Parameter:
	*	floor: an unsigned int for the current floor number.
	*/
	void make_collectible(unsigned int floor);

	/**
	* Make gold collectibles for the current floor.
	*
	* Parameter:
	*	floor: an unsigned int for the current floor number.
	*/
	void make_gold(unsigned int floor);

	/**
	* Make interactibles for the current floor.
	*
	* Parameter:
	*	floor: an unsigned int for the current floor number.
	*/
	void make_interactible(unsigned int floor);

	/**
	* Make the map for the current floor.
	*/
	void make_map();

	/**
	* Checks if the stair intersect with a rectangle.
	* 
	* Parameter:
	*	rect: a sf::FloatRect for the rectangle.
	* 
	* Return:
	*	true if an intersection exists.
	*/
	bool stair_intersect(const sf::FloatRect& rect);

	/**
	* Checks if the shop intersect with a rectangle.
	* 
	* Parameter:
	*	rect: a sf::FloatRect for the rectangle.
	* 
	* Return:
	*	true if an intersection exists.
	*/
	bool shop_intersect(const sf::FloatRect& rect);
};

#endif