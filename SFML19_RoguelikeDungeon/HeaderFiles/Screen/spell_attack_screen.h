#include "screen.h"
#include <Shape/full_rectangle.h>

#ifndef SPELL_ATTACK_SCREEN_H
#define SPELL_ATTACK_SCREEN_H

/**
* Represents the spell attack dialog scene.
*/
class Spell_Attack_Screen : public Screen {
private:
	static int spellID;
	static Full_Rectangle rangeBox;

	/*
	* Change range display of spell.
	*/
	void changeRange();

	/*
	* Reset spell select.
	*/
	void resetSpell();

public:
	Spell_Attack_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void draw();
};

#endif