#include "stat.h"

#ifndef EFFECT_H
#define EFFECT_H

/**
* A struct that represents an effect which modifies player's stat for a specified amount of turns.
*/
struct Effect {
	Stat stat_changed;
	int stat_difference;
	unsigned change_turns;
	unsigned original_turns;
	bool effect_applied = false;

	/**
	* Constructor for Effect where original turns = change turns.
	*/
	Effect(Stat, int, unsigned int);

	/**
	* Constructor for Effect where original turns != change turns.
	*/
	Effect(Stat, int, unsigned int, unsigned int);
};

#endif