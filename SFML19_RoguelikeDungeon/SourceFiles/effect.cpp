/**
*
* File: effect.cpp
* Description: Implementations of the Effect struct.
*
*/

#include "effect.h"

Effect::Effect(unsigned int change, int difference, unsigned int turn) :
	stat_changed(change), stat_difference(difference), change_turns(turn),
	original_turns(turn) {
}