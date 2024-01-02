/**
*
* File: spell_class.h
* Description: This class abstracts a spell which player can utilize to provide special effects in the game world.
* Also contains child classes from the Spell class to contain  individual spells.
*
*/

#ifndef SPELL_H
#define SPELL_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "player_class.h"
#include "enemies_class.h"
#include "floor_room_class.h"

class Spell {
protected:
	// use, 0: items targeting the floor, 1: enemies and players, 2: enemies, 3: players, 4: spell attack on map
	unsigned int id{ 0 }, uses{ 0 }, range{ 0 }, sell_gd{ 0 }, buy_gd{0};
	sf::Text icon, desc, txt;

public:

	/**
	* Constructor for Spell. Used mainly for PH_Spell
	*/
	Spell();

	/**
	* Constructor for Spell.
	* 
	* Parameter:
	*	font: a sf::Font with data about a font. Sets the font of spell.
	*	abbre: a const char[3] with the characters for the spell abbreviation.
	*	t_id: an unsigned int for the spell's id.
	* 	t_buy: an unsigned int for the spell's buy gold.
	* 	t_sell: an unsigned int for the spell's sell gold.
	*  	t_use: an unsigned int for the spell's usage type.
	*/
	Spell(sf::Font& font, const char abbre[3], 
		unsigned int t_id, 
		unsigned int t_buy, 
		unsigned int t_sell, 
		unsigned int t_use);

	/**
	* setter for the position for spell slot.
	* 
	* Parameter:
	*	x: set the slot's x position to int x
	*	y: set the slot's x position to int y
	*/
	void set_pos(int x, int y);

	/**
	* getter for the position for spell slot.
	*
	* Parameter:
	*	z: char. 'x' to get the x position. 'y' for y position.
	* 
	* Return:
	*	an int value for the position.
	*/
	int get_pos(char z);

	/**
	* A virtual method to be implemented by child class. This method is
	* what a spell would do.
	*
	* Parameter:
	*	floor: Pass in the Floor object for spell to modify.
	*	player: Pass in the Player object for spell to modify.
	*	enemies: A vector of Enemy objects for spell to modify.
	*/
	virtual void use(Floor &floor, Player &player, std::vector<Enemy>& enemies);

	/**
	* A virtual method to be implemented by child class. This method returns
	* information on the attack value, range, physical/magic type, && cost of spell.
	*
	* Parameter:
	*	player: A Player object that will be affected by the spell.
	* 
	* Return:
	*	a std::array<int, 4>. index 0 = attack value, 1 = range, 2 = (physical or magic) 2/3, 3 = cost
	*/
	virtual std::array<int, 4> atk(Player& player);

	/**
	* Getter for spell's id.
	* 
	* Return:
	*	An int for spell's id.
	*/
	unsigned int get_id();

	/**
	* Getter for spell's usage type.
	* 
	* Return:
	*	An int for spell's usage type.
	*/
	unsigned int get_use();

	/**
	* Getter for spell's sell gold.
	* 
	* Return:
	*	An int for spell's sell gold.
	*/
	unsigned int get_sell_gd();

	/**
	* Getter for spell's buy gold.
	*
	* Return:
	*	An int for spell's buy gold.
	*/
	unsigned int get_buy_gd();

	/**
	* Static method to create a spell for game usage.
	*
	* Parameter:
	*	id: An unsigned int for id of spell to search for && create
	*	font: An sf::Font for font type to pass in when creating spell.
	* 
	* Return:
	*	A Spell* containing the spell data.
	*/
	static Spell* create_spell(unsigned int id, sf::Font& font);

	/**
	* Draw many aspects of the spell on the SFML window.
	*
	* Parameter:
	*	window: A sf::RenderWindow object to display the spell icon.
	*	type: a char. 't' for icon. 'd' for desc. 's' for shortcut.
	*/
	void draw(sf::RenderWindow& window, char type);
};

class PH_Spell : public Spell {
public:
	PH_Spell();
};

class Heal : public Spell {
public:
	Heal(sf::Font& font);

	void use(Floor& floor, Player& player, std::vector<Enemy>& enemies);
};

class Damage_All : public Spell {
public:
	Damage_All(sf::Font& font);

	void use(Floor& floor, Player& player, std::vector<Enemy>& enemies);
};

class Fire : public Spell {
public:
	Fire(sf::Font& font);

	std::array<int, 4> atk(Player& player);
};

#endif