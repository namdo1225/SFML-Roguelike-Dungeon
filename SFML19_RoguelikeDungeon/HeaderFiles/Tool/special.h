/**
*
* File: special.h
* Description: This class abstracts a special item/perk that players can get like increasing inventory limits.
*
*/

#ifndef SPECIAL_H
#define SPECIAL_H

#include <SFML/Graphics.hpp>
#include "player.h"
#include "Floor/enemy.h"
#include "Floor/floor.h"
#include "Shape/full_text.h"
#include "Shape/full_rectangle.h"

class Special : public Tool {
public:
	const static unsigned int SPECIALS = 6;

	/**
	* A virtual method to be implemented by child class. This method is
	* what a special would do.
	*/
	virtual void use() = 0;

	/**
	* Constructor for Special.
	* 
	* Parameter:
	* 	id: an unsigned int for the spell's id.
	* 	buy: an unsigned int for the spell's buy gold.
	* 	sell: an unsigned int for the spell's sell gold.
	*	abbre: a const char[3] for the items' name abbreviation.
	*	desc: a const char* for the item's description.
	*	name: a const char* for item's name.
	*/
	Special(unsigned int id, unsigned int buy, unsigned int sell,
		const char abbre[3], const char* desc, const char* name);

	static std::shared_ptr<Special> create_special(unsigned int id);
};

class PH_Special : public Special {
public:
	PH_Special();

	virtual void use();
};

class Storage_Perk : public Special {
public:
	Storage_Perk();

	virtual void use();
};

class Floor_Perk : public Special {
public:
	Floor_Perk();

	virtual void use();
};

class Strength_Perk : public Special {
public:
	Strength_Perk();

	virtual void use();
};

class Magic_Perk : public Special {
public:
	Magic_Perk();

	virtual void use();
};

class Defense_Perk : public Special {
public:
	Defense_Perk();

	virtual void use();
};

class Resistance_Perk : public Special {
public:
	Resistance_Perk();

	virtual void use();
};

#endif