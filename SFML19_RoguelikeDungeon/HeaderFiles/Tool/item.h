/**
*
* File: item.h
* Description: This class abstracts an item which player can use to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual items.
*
*/

#include "stat.h"
#include "tool.h"
#include <functional>
#include <map>

#ifndef ITEM_H
#define ITEM_H

enum ItemType { Weapon, Armor, StatConsumable, UtilityConsumable };

class Item : public Tool {
protected:
	std::function<void()> utilityUse = NULL;

public:
	static std::map<unsigned int, Item> items;
	Stat stat{ Max_Hp };
	ItemType type{ Weapon };

	/**
	* Constructor for Item.
	* 
	* Parameter:
	*	id: an int for the item's id.
	* 	type: an int for the item's type.
	* 	modify_act: a bool. true for item that modify a stat. false for item that can be used without modifying stat.
	* 	hp_mp_other: a bool. false for modifying hp/mp. true for other stat.
	* 	stat: a Stat enum for the type of stat modified.
	* 	quantity: an int for the number that the item is supposed to have. 
	*					Ex: if it's a health potion, the number is the amount the potion heals.
	* 	range: an int for the item's range.
	* 	buy: an int for the item's buy gold.
	* 	sell: an int for the item's sell gold.
	*	abbre: a const char[3] for the items' name abbreviation.
	*	desc: a const char* for the item's description.
	*	name: a const char* for item's name.
	*/
	Item(unsigned int id, ItemType type, Stat stat, int quantity,
		unsigned int range, unsigned int buy, unsigned int sell,
		const char abbre[3], const char* desc, const char* name, std::function<void()> utilityUse = NULL);

	Item();

	Item(unsigned int id);

	static bool setup();

	/**
	* Use the item.
	*/
	void use();
};

#endif