#include "stat.h"
#include "tool.h"
#include <functional>
#include <map>
#include <string>

#ifndef ITEM_H
#define ITEM_H

/**
* StatConsumable are consumables that modify a stat.
* UtilityConsumable are non-Stat consumables.
*/
enum ItemType { Weapon, Armor, StatConsumable, UtilityConsumable };

/**
* This class abstracts an item which player can use to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual items.
*/
class Item : public Tool {
private:
	Stat stat{ Max_Hp };

	ItemType type{ Weapon };

	std::function<void()> utilityUse = NULL;

public:
	static std::map<unsigned int, Item> items;

	/**
	* Constructor for Item.
	* 
	* Parameter:
	*	id: item's id.
	* 	type: item's type.
	* 	stat: Stat that will be modified.
	* 	quantity: an int for the number that the item is supposed to have.
	*		Ex: if it's a health potion, the number is the amount the potion heals.
	* 	range: item's range.
	* 	buy: item's buy gold.
	* 	sell: item's sell gold.
	*	abbre: a const char[3] for the items' name abbreviation.
	*	desc: a const char* for the item's description.
	*	name: a const char* for item's name.
	*	utilityUse: a function that can be called when utility consumable is used.
	*/
	Item(unsigned int id, ItemType type, Stat stat, int quantity,
		unsigned int range, unsigned int buy, unsigned int sell,
		std::string abbre, std::string desc, std::string name, std::function<void()> utilityUse = NULL);

	/**
	* Constructor for Item.
	*/
	Item();

	/**
	* Constructor for Item.
	* 
	* Parameter:
	*	id: an id to fill item with information.
	*/
	Item(unsigned int id);

	/**
	* Get items from the database.
	*
	* Return:
	*	true if setup is successful.
	*/
	static bool setup();

	/**
	* Use the item.
	*/
	void use();

	unsigned int getType();

	/**
	* Getter for item's stat.
	*
	* Return:
	*	item's stat.
	*/
	Stat getStat();
};

#endif