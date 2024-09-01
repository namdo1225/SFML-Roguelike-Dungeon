#include "screen.h"
#include <array>
#include <Shape/full_rectangle.h>

#ifndef SPELL_ATTACK_SCREEN_H
#define SPELL_ATTACK_SCREEN_H

/**
* Represents the spell attack dialog scene.
*/
class Spell_Attack_Screen : public Screen {
private:
	static std::array<int, 3> atkSpInfo;
	static Full_Rectangle ranges[4];

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