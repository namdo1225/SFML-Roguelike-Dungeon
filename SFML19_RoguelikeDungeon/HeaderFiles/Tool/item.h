/**
*
* File: item.h
* Description: This class abstracts an item which player can use to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual items.
*
*/

#include "Shape/full_text.h"
#include "Shape/full_rectangle.h"
#include "player.h"
#include "stat.h"
#include "tool.h"

#ifndef ITEM_H
#define ITEM_H

class Item : public Tool {
protected:
	// for member type: 0 = non-equipable, 1 = weapon, 2 = armor, 3 = item does not exist

	Stat stat{ Max_Hp };

	// modify_or_act, true: modify player's stats or hp/mp, false: this is an item that doesn't modify stats
	// hp_mp_other, true: modify player's stats, false: modify player's hp or mp
	bool modify_or_act{}, hp_mp_other{};

	// ex: for healing potion, it would be amounts healed. For str, it would be the amount it increases by.
	int quantity{};

public:
	const static unsigned int ITEMS = 9;

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
	*/
	Item(int id, int type, bool modify_act,
		bool hp_mp_other, Stat stat, int quantity,
		int range, int buy, int sell, const char abbre[3], const char* desc);

	/**
	* A virtual method to be implemented by child class. This method is
	* what an item would do.
	*
	* Parameter:
	*	id: an int for item's id.
	* 
	* Return:
	*	An std::shared_ptr<Item> object for the new item.
	*/
	static std::shared_ptr<Item> create_itm(unsigned int id);

	/**
	* Getter for item's stat type.
	*
	* Return:
	*	A Stat enum for item's stat type.
	*/
	Stat get_stat();

	/**
	* Getter for item's quantity.
	*
	* Return:
	*	An int for item's quantity.
	*/
	int get_quantity();

	/**
	* Getter for whether item modifies hp/mp or other stats.
	*
	* Return:
	*	An bool. true if item modifies other stats. false otherwise.
	*/
	bool get_hp_mp_other();

	/**
	* A virtual method to be implemented by child class. This method is
	* what an item would do.
	*
	* Parameter:
	*	player: Pass in the Player object for item to modify.
	*/
	virtual void use();
};

class Place_Holder : public Item {
public:
	Place_Holder();
};

class Iron_Sword : public Item {
public:
	Iron_Sword();
};

class Iron_Armor : public Item {
public:
	Iron_Armor();
};

class Health_Potion : public Item {
public:
	Health_Potion();

	void use();
};

class Magic_Armor : public Item {
public:
	Magic_Armor();
};

class Mana_Potion : public Item {
public:
	Mana_Potion();

	void use();
};

class Wooden_Staff : public Item {
public:
	Wooden_Staff();
};

class Steel_Sword : public Item {
public:
	Steel_Sword();
};

class Mage_Staff : public Item {
public:
	Mage_Staff();
};


class Steel_Armor : public Item {
public:
	Steel_Armor();
};

#endif