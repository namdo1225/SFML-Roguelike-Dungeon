/**
*
* File: spell.h
* Description: This class abstracts a spell which player can utilize to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual spells.
*
*/

#ifndef SPELL_H
#define SPELL_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "player.h"
#include "Floor/enemy.h"
#include "Floor/floor.h"
#include "Shape/full_text.h"
#include "Shape/full_rectangle.h"

class Spell : public Tool {
protected:
	// use, 0: items targeting the floor, 1: enemies and players, 2: enemies, 3: players, 4: spell attack on map
	unsigned int uses{ 0 }, mp{ 0 };

	// represents the value of the spell like for example attack damage against enemy
	unsigned int quantity{ 0 };

	// type member: 2 is a physical damage spell, 3 is a magical damage spell

public:
	const static unsigned int SPELLS = 3;

	/**
	* Constructor for Spell.
	* 
	* Parameter:
	*	abbre: a const char[3] with the characters for the spell abbreviation.
	*	id: an unsigned int for the spell's id.
	* 	buy: an unsigned int for the spell's buy gold.
	* 	sell: an unsigned int for the spell's sell gold.
	*  	use: an unsigned int for the spell's usage type.
	*	range: unsigned int for spell's range.
	*	type: unsigned int for spell's type.
	*	mp: unsigned int for spell's mp use.
	*	desc: a const char* for the spell's description.
	*/
	Spell(const char abbre[3],
		unsigned int id, unsigned int buy, unsigned int sell, unsigned int use,
		unsigned int range, unsigned int type, unsigned int mp,
		const char* desc);

	/**
	* A virtual method to be implemented by child class. This method is
	* what a spell would do.
	*/
	virtual void use();

	/**
	* A virtual method to be implemented by child class. This method returns
	* information on the attack value, range, physical/magic type, && cost of spell.
	* 
	* Return:
	*	a std::array<int, 4>. index 0 = attack value, 1 = range, 2 = (physical or magic) 2/3, 3 = cost
	*/
	virtual std::array<int, 4> atk();

	/**
	* Getter for spell's usage type.
	* 
	* Return:
	*	An int for spell's usage type.
	*/
	unsigned int get_use();

	/**
	* Static method to create a spell for game usage.
	*
	* Parameter:
	*	id: An unsigned int for id of spell to search for && create
	* 
	* Return:
	*	A std::shared_ptr<Spell> containing the spell data.
	*/
	static std::shared_ptr<Spell> create_spell(unsigned int id);
};

class PH_Spell : public Spell {
public:
	PH_Spell();
};

class Heal : public Spell {
public:
	Heal();

	void use();
};

class Damage_All : public Spell {
public:
	Damage_All();

	void use();
};

class Fire : public Spell {
public:
	Fire();

	std::array<int, 4> atk();
};

#endif