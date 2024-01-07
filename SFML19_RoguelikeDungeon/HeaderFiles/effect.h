/**
*
* File: effect.h
* Description: Defines a struct that represents an effect which modifies player's stat for a specified amount of turns.
*
*/

#ifndef EFFECT_H
#define EFFECT_H

struct Effect {
	unsigned int stat_changed;
	int stat_difference;
	unsigned change_turns;
	unsigned original_turns;
	bool effect_applied = false;

	Effect(unsigned int, int, unsigned int);
};

#endif