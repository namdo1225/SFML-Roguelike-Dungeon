#include "Shape/full_rectangle.h"
#include "Shape/full_text.h"
#include <SFML/Graphics/Color.hpp>
#include <string>

#ifndef TOOL_H
#define TOOL_H

/**
* This parent class abstracts a tool that the player can use (whether it's an item or tool).
*/
class Tool {
protected:
	Full_Text icon;

	unsigned int id = 0, range = 0, buy = 0, sell = 0;
	std::string name = "", originalDesc = "", desc = "", abbrev = "";

	/**
	* Represents tool's value (eg. spell attack damage against enemy).
	* For healing potion, it would be amounts healed.
	*/
	int quantity{ 0 };

	/**
	* Useful for boundary detection with the mouse.
	*/
	Full_Rectangle boundRect = Full_Rectangle(-100.f, -100.f, 60.f, 60.f, false, true, sf::Color::Transparent,
		sf::Color::Transparent);

	/**
	* Constructor for Tool.
	* 
	* Parameter:
	*	name: the full name of the tool.
	*	desc: the description of the tool.
	*	abbrev: the shortened name of the tool.
	*	id: tool's id.
	*	buy: tool's buy price.
	*	sell: tool's sell price.
	*	quanitty: tool's known quantity (can represent spell dmg for example).
	*	range: tool's range if applicable.
	*/
	Tool(std::string name, std::string desc, std::string abbrev,
		unsigned int id, unsigned int buy, unsigned int sell, int quantity,
		unsigned int range = 0);

public:
	/**
	* Draw many aspects of the item on the SFML window.
	*/
	void draw() const;

	/**
	* Getter for the position for tool slot.
	*
	* Parameter:
	*	z: 'x' to get the x position. 'y' for y position.
	*
	* Return:
	*	a value for the position.
	*/
	int getPos(char z);

	/**
	* Setter for the position for tool slot.
	*
	* Parameter:
	*	x: set the slot's x position.
	*	y: set the slot's y position.
	*/
	void setPos(float x, float y);

	/**
	* Whether the tool contains the passed coordinate.
	* 
	* Parameter:
	*	x: The x coordinate.
	*	y: The y coordinate.
	* 
	* Return:
	*	true if the tool icon contains the coordinate.
	*/
	bool contains(float x, float y);

	/**
	* Change the SFML components' theme.
	*/
	void changeTheme();

	/**
	* Getter for tool's name.
	* 
	* Return:
	*	tool's name.
	*/
	std::string getName();

	/**
	* Getter for tool's original description.
	*
	* Return:
	*	tool's original description.
	*/
	std::string getOriginalDesc();

	/**
	* Getter for tool's description.
	*
	* Return:
	*	tool's description.
	*/
	std::string getDesc();

	/**
	* Getter for tool's abbreviation.
	*
	* Return:
	*	tool's abbreviation.
	*/
	std::string getAbbrev();
	
	/**
	* Getter for tool's id.
	*
	* Return:
	*	tool's id.
	*/
	unsigned int getID();

	/**
	* Getter for tool's range.
	*
	* Return:
	*	tool's range.
	*/
	unsigned int getRange();

	/**
	* Getter for tool's buy gold.
	*
	* Return:
	*	tool's buy gold.
	*/
	unsigned int getBuy();

	/**
	* Getter for tool's sell gold.
	*
	* Return:
	*	tool's sell gold.
	*/
	unsigned int getSell();

	/**
	* Getter for tool's quantity.
	*
	* Return:
	*	tool's quantity.
	*/
	int getQuantity();

	/**
	* Getter for tool's type.
	*
	* Return:
	*	tool's type.
	*/
	virtual unsigned int getType();
};

#endif