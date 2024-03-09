/**
*
* File: item.h
* Description: This class abstracts an item which player can use to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual items.
*
*/

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "player.h"
#include "stat.h"

#ifndef ITEM_H
#define ITEM_H

class Item {
protected:
	sf::Text txt, icon, description;

	// type: 0 = non-equipable, 1 = weapon, 2 = armor, 3 = item does not exist
	unsigned int type{ 3 };
	unsigned int id, range, sell_gd, buy_gd = 0;
	Stat stat{ Max_Hp };

	// modify_or_act, true: modify player's stats or hp/mp, false: this is an item that doesn't modify stats
	// hp_mp_other, true: modify player's stats, false: modify player's hp or mp
	bool modify_or_act{}, hp_mp_other{};

	// ex: for healing potion, it would be amounts healed. For str, it would be the amount it increases by.
	int quantity{};

public:

	/**
	* Constructor for Item.
	*/
	Item();

	/**
	* Constructor for Item.
	* 
	* Parameter:
	*	t_id: an int for the item's id.
	* 	t_type: an int for the item's type.
	* 	t_modify_act: a bool. true for item that modify a stat. false for item that can be used without modifying stat.
	* 	t_hp_mp_other: a bool. false for modifying hp/mp. true for other stat.
	* 	t_stat_type: a Stat enum for the type of stat modified.
	* 	t_modified_num: an int for the number that the item is supposed to have. 
	*					Ex: if it's a health potion, the number is the amount the potion heals.
	* 	t_range: an int for the item's range.
	* 	t_buy: an int for the item's buy gold.
	* 	t_sell: an int for the item's sell gold.
	*	abbre: a const char[3] for the items' name abbreviation.
	* 	font: a sf::Font object that holds the font data for the item's texts.
	*/
	Item(int t_id, int t_type, bool t_modify_act,
		bool t_hp_mp_other, Stat t_stat_type, int t_modified_num,
		int t_range, int t_buy, int t_sell, const char abbre[3],
		sf::Font& font);

	/**
	* Draw many aspects of the item on the SFML window.
	*
	* Parameter:
	*	window: A sf::RenderWindow object to display the item icon.
	*	type: a char. 't' for icon. 'd' for desc. 's' for shortcut.
	*/
	void draw(sf::RenderWindow& window, char type);

	/**
	* Getter for the position for item slot.
	*
	* Parameter:
	*	z: char. 'x' to get the x position. 'y' for y position.
	*
	* Return:
	*	an int value for the position.
	*/
	int get_pos(char z);

	/**
	* Getter for item's sell gold.
	* 
	* Return:
	*	An int for item's sell gold.
	*/
	unsigned int get_sell_gd();

	/**
	* Getter for item's buy gold.
	*
	* Return:
	*	An int for item's buy gold.
	*/
	unsigned int get_buy_gd();

	/**
	* Getter for item's id.
	* 
	* Return:
	*	An int for item's id.
	*/
	unsigned int get_id();

	/**
	* Setter for the position for spell slot.
	* 
	* Parameter:
	*	x: set the slot's x position to int x
	*	y: set the slot's x position to int y
	*/
	void set_pos(int x, int y);

	/**
	* Getter for item's type.
	*
	* Return:
	*	An int for item's type.
	*/
	unsigned int get_type();

	/**
	* Getter for item's range.
	*
	* Return:
	*	An int for item's range.
	*/
	unsigned int get_range();

	/**
	* A virtual method to be implemented by child class. This method is
	* what an item would do.
	*
	* Parameter:
	*	id: an int for item's id.
	*	font: sf::Font object for font data to be used for the new item.
	* 
	* Return:
	*	An Item* object (subclass of object), which is the new item.
	*/
	static Item* create_itm(unsigned int id, sf::Font& font);

	/**
	* Getter for item's abbreviation letters.
	*
	* Return:
	*	An int for item's abbreviation letters.
	*/
	std::string get_letter();

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
	*	player: Pass in the Player object for spell to modify.
	*/
	virtual void use(Player &player);
};

class Place_Holder : public Item {
public:
	Place_Holder();
};

class Iron_Sword : public Item {
public:
	Iron_Sword(sf::Font &font);
};

class Iron_Armor : public Item {
public:
	Iron_Armor(sf::Font& font);
};

class Health_Potion : public Item {
public:
	Health_Potion(sf::Font& font);

	void use(Player& player);
};

class Magic_Armor : public Item {
public:
	Magic_Armor(sf::Font& font);
};

class Mana_Potion : public Item {
public:
	Mana_Potion(sf::Font& font);

	void use(Player& player);
};

class Wooden_Staff : public Item {
public:
	Wooden_Staff(sf::Font& font);
};

#endif