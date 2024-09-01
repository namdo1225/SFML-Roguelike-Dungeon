/**
*
* File: spell.h
* Description: This class abstracts a spell which player can utilize to provide special effects in the game world.
* Also contains child classes from the Spell class to contain individual spells.
*
*/

#ifndef SPELL_H
#define SPELL_H

#include "tool.h"
#include <array>
#include <functional>
#include <map>
#include <string>

enum SpellType{Functional, Offensive};

class Spell : public Tool {
private:
	bool usePlayerMP() const;

protected:
	std::function<void(int, double)> functionalUse = NULL;

public:
	static std::map<unsigned int, Spell> spells;

	SpellType type{ Functional };
	unsigned int mp{ 0 };

	/*
	* Scaling for the stats for attack spells;
	*/
	double percentage = 0.5;

	/**
	* Constructor for Spell.
	* 
	* Parameter:
	*	abbre: a const char[3] with the characters for the spell abbreviation.
	*	id: Spell's id.
	* 	buy: Spell's buy gold.
	* 	sell: Sspell's sell gold.
	*  	use: Spell's usage type.
	*	range: Spell's range.
	*	type: Spell's type.
	*	mp: Spell's mp use.
	*	desc: Spell's description.
	*	name: Spell's name.
	*/
	Spell(std::string abbre,
		unsigned int id, unsigned int buy, unsigned int sell, SpellType use,
		unsigned int range, unsigned int mp, int quantity,
		std::string desc, std::string name, double percent = 0.5f, std::function<void(int, double)> functionalUse = NULL);

	Spell();

	Spell(unsigned int id);

	static bool setup();

	/**
	* Use a functional spell.
	* 
	* Return:
	*	true if spell executed successfully. false if execution failed.
	*/
	bool use() const;

	/**
	* Use an offensive spell.
	* 
	* Return:
	*	index 0 = attack value, 1 = range, 2 = cost
	*/
	std::array<int, 3> atk();
};

#endif