#include "tool.h"
#include <functional>
#include <map>
#include <string>

#ifndef SPELL_H
#define SPELL_H

/**
* Offensive spells require spell attack screen. Functional spells
* are non-Offensive spells.
*/
enum SpellType{Functional, Offensive};

/**
* This class abstracts a spell which player can utilize to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual spells.
*/
class Spell : public Tool {
private:
	/**
	* Use the player's MP.
	* 
	* Return:
	*	true if player has the MP to use the spell.
	*/
	bool usePlayerMP() const;

	SpellType type{ Functional };

	unsigned int mp{ 0 };

	/*
	* Scaling for the stats for attack spells;
	*/
	double percentage = 0.5;

protected:
	std::function<void(int, double)> functionalUse = NULL;

public:
	static std::map<unsigned int, Spell> spells;

	/**
	* Constructor for Spell.
	* 
	* Parameter:
	*	abbre: a const char[3] with the characters for the spell abbreviation.
	*	id: Spell's id.
	* 	buy: Spell's buy gold.
	* 	sell: Spell's sell gold.
	*  	use: Spell's usage type.
	*	range: Spell's range.
	*	type: Spell's type.
	*	mp: Spell's mp use.
	*	desc: Spell's description.
	*	name: Spell's name.
	*	functionalUse: function that can be called when spell is used.
	*/
	Spell(std::string abbre,
		unsigned int id, unsigned int buy, unsigned int sell, SpellType use,
		unsigned int range, unsigned int mp, int quantity,
		std::string desc, std::string name, double percent = 0.5f, std::function<void(int, double)> functionalUse = NULL);

	/**
	* Constructor for Spell.
	*/
	Spell();

	/**
	* Constructor for Spell.
	* 
	* Parameter:
	*	id: an id to fill spell with information.
	*/
	Spell(unsigned int id);

	/**
	* Get spells from the database.
	* 
	* Return:
	*	true if setup is successful.
	*/
	static bool setup();

	/**
	* Use a functional spell.
	* 
	* Return:
	*	true if spell executed successfully.
	*/
	bool use() const;

	unsigned int getType();

	/**
	* Getter for spell's mp use.
	*
	* Return:
	*	spell's mp use.
	*/
	unsigned int getMP();

	/**
	* Getter for spell's percentage scaling.
	*
	* Return:
	*	spell's percentage scaling.
	*/
	double getPercentage();
};

#endif