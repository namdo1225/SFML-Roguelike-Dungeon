/**
*
* File: tool.h
* Description: This parent class abstracts a tool that the player can use (whether it's an item or tool).
*
*/

#include "Shape/full_text.h"
#include "Shape/full_rectangle.h"
#include "player.h"
#include "stat.h"

#ifndef TOOL_H
#define TOOL_H

class Tool {
protected:
	const static char* STAT_STRINGS[];
	const char* name;

	Full_Text txt, icon, desc;
	Full_Rectangle invisible_rect = Full_Rectangle(-100.f, -100.f, 60.f, 60.f, false, true, sf::Color::Transparent,
		sf::Color::Transparent);

	unsigned int id = 0, range = 0, sell_gd = 0, buy_gd = 0, type = 0;

	// represents the value of the tool like for example: spell attack damage against enemy.
	// For healing potion, it would be amounts healed.
	int quantity{ 0 };

	/**
	* Constructor for Tool.
	*/
	Tool();

public:
	/**
	* Draw many aspects of the item on the SFML window.
	*
	* Parameter:
	*	window: A sf::RenderWindow object to display the item icon.
	*	type: a char. 't' for icon. 'd' for desc. 's' for shortcut.
	*/
	void draw(char type);

	/**
	* Getter for the position for tool slot.
	*
	* Parameter:
	*	z: char. 'x' to get the x position. 'y' for y position.
	*
	* Return:
	*	an int value for the position.
	*/
	int get_pos(char z);

	/**
	* Getter for tool's sell gold.
	*
	* Return:
	*	An int for tool's sell gold.
	*/
	unsigned int get_sell_gd();

	/**
	* Getter for tool's buy gold.
	*
	* Return:
	*	An int for tool's buy gold.
	*/
	unsigned int get_buy_gd();

	/**
	* Getter for tool's id.
	*
	* Return:
	*	An int for tool's id.
	*/
	unsigned int get_id();

	/**
	* Setter for the position for tool slot.
	*
	* Parameter:
	*	x: set the slot's x position to int x
	*	y: set the slot's x position to int y
	*/
	void set_pos(int x, int y);

	/**
	* Getter for item's abbreviation letters.
	*
	* Return:
	*	An int for item's abbreviation letters.
	*/
	std::string get_letter();

	/**
	* Whether the tool contains the passed position.
	*/
	bool contains(float x, float y);

	void change_theme();

	/**
	* Getter for tool's range.
	*
	* Return:
	*	An int for tool's range.
	*/
	unsigned int get_range();

	/**
	* Getter for tool's type.
	*
	* Return:
	*	An int for tool's type.
	*/
	unsigned int get_type();

	/**
	* Getter for tool's name.
	*
	* Return:
	*	A const char* for tool's name.
	*/
	const char* get_name();

	/**
	* Getter for tool's quantity.
	*
	* Return:
	*	An int for tool's quantity.
	*/
	int get_quantity();
};

#endif