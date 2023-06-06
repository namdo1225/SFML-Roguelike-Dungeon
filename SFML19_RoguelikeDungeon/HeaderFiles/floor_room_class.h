/**
*
* File: floor_room_class.c
* Description: Contain Floor and Room classes, which represents a dungeon floor and
* the various rooms on that dungeon.
*
*/

#ifndef FLOOR_H
#define FLOOR_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "items_class.h"
#include <array>

class Collectible {
private:

	unsigned int id{ 0 };
	sf::RectangleShape rect;
	static sf::Texture col_tex;

public:

	/**
	* Constructor for Collectible.
	*/
	Collectible();

	/**
	* Constructor for Collectible.
	* 
	* Parameter:
	*	x: an int for the item's x position.
	* 	y: an int for the item's x position.
	*	t_id: an unsigned int for the item's id.
	*/
	Collectible(int x, int y, unsigned int t_id);

	/**
	* Getter for item's position.
	*
	* Parameter:
	*	z: a char. 'x' to get item's x position. 'y' to get item's y position.
	* 
	* Return:
	*	an int for the item's x or y position.
	*/
	int get_pos(char z);

	/**
	* Getter for item's id.
	*
	* Return:
	*	an unsigned int for the item's id.
	*/
	unsigned int get_id();

	/**
	* Setter for item's position.
	*
	* Parameter:
	*	x: an int for the item's new x position.
	*	y: an int for the item's new y position. 
	*/
	void set_pos(int x, int y);

	/**
	* Draw the item on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the item will be drawn on.
	*/
	void draw(sf::RenderWindow& window);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

class Gold_Collectible {
private:

	sf::RectangleShape rect;
	unsigned int amount{ 0 };

	static sf::Texture gold_tex;

public:

	/**
	* Constructor for Gold_Collectible.
	*/
	Gold_Collectible();

	/**
	* Constructor for Gold_Collectible.
	*
	* Parameter:
	* 	floor: an unsigned int for the current floor.
	* 	t_amount: an unsigned int for the gold amount.
	*	x: an int for the gold's x position.
	* 	y: an int for the gold's x position.
	*/
	Gold_Collectible(unsigned int floor, unsigned int t_amount, int x, int y);

	/**
	* Getter for the amount.
	*
	* Return:
	*	an unsigned int for the gold amount.
	*/
	unsigned int get_amount();

	/**
	* Setter for gold's position.
	*
	* Parameter:
	*	x: an int for the gold's new x position.
	*	y: an int for the gold's new y position.
	*/
	void set_pos(int x, int y);

	/**
	* Getter for gold's position.
	*
	* Parameter:
	*	z: a char. 'x' to get gold's x position. 'y' to get gold's y position.
	*
	* Return:
	*	an int for the gold's x or y position.
	*/
	int get_pos(char z);

	/**
	* Draw the gold on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the gold will be drawn on.
	*/
	void draw(sf::RenderWindow& window);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

class Stair {
public:
	sf::RectangleShape rect;
	static sf::Texture stair_tex;

	/**
	* Constructor for Stair.
	*/
	Stair();

	/**
	* Constructor for Stair.
	*
	* Parameter:
	*	x: an int for the stair's x position.
	* 	y: an int for the stair's x position.
	*/
	Stair(int x, int y);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

class Shop {
public:
	sf::RectangleShape rect;
	static sf::Texture shop_tex;

	/**
	* Constructor for Shop.
	*/
	Shop();

	/**
	* Constructor for Shop.
	*
	* Parameter:
	*	x: an int for the shop's x position.
	* 	y: an int for the shop's x position.
	*/
	Shop(int x, int y);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

class Room {
private:

	static sf::Texture room_tex;

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
	* Setter for room's position and size.
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
	*	purpose: an int. 0, 2 = horizontal door rotation. 1, 3 = vertical door rotation.
	*	-1 if you just want to change the door's position.
	*	-2 to change door's size.
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
	*	an int for the shop's x or y position.
	*/
	int get_rm(char z);

	/**
	* Getter for information about the door.
	*
	* Parameter:
	*	z: a char. 'x' to get door's x position. 'y' to get door's y position.
	*		'w' for width. 'h' for height. '0' - '3' for doors' filled slot.
	*	an int for the shop's x or y position.
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

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

class Floor {
private:

	bool fl_shop{ false };
	Shop shop;
	Stair stair;

public:

	std::vector<Room> rooms;
	std::vector<Room> rooms_map;
	std::vector<Collectible> collectibles;
	std::vector<Gold_Collectible> golds;

	/**
	* Constructor for Floor.
	*/
	Floor();

	/**
	* Helper method to set a room's position and size.
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
	* Helper method to set door's position (can be either x or y).
	* 
	* Parameter:
	*	rand_door_coord: an int& for the coordinate that will be set as the door's coordinate (can be either x or y).
	*	rand_coord: an int for the position of rand_door_coord.
	*	rand_size: an int for the size of rand_door_coord.
	*/
	void set_door_pos_1(int& rand_door_coord, int rand_coord, int rand_size);

	/**
	* Helper method to set door's position (can be either x or y).
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
	*	an int for the shop's x or y position.
	*/
	int get_shop_pos(char z);

	/**
	* Getter for stair's position.
	*
	* Parameter:
	*	z: a char. 'x' for the stair's x position. 'y' for the stair's y position.
	*
	* Return:
	*	an int for the stair's x or y position.
	*/
	int get_stair_pos(char z);

	/**
	* Draw the golds, collectibles, rooms, and doors on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the map will be drawn on.
	*/
	void draw(sf::RenderWindow& window);

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
	* Make the map for the current floor.
	*/
	void make_map();

	/**
	* Draw the map on the game's window.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the map will be drawn on.
	*/
	void draw_map(sf::RenderWindow& window);
};

#endif