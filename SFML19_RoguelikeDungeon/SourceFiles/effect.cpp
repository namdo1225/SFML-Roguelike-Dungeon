/**
*
* File: effect.cpp
* Description: Implementations of the Effect struct.
*
*/

#include "effect.h"
#include <stat.h>

Effect::Effect(Stat change, int difference, unsigned int turn) :
	stat_changed(change), stat_difference(difference), change_turns(turn),
	original_turns(turn) {
}

Effect::Effect(Stat change, int difference, unsigned int original, unsigned int current) :
	stat_changed(change), stat_difference(difference), change_turns(current),
	original_turns(original) {
}
