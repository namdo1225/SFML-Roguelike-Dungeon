/**
*
* File: spell_attack_screen.h
* Description: Contain the declaration of the Spell_Attack_Screen class,
* which represents the spell attack dialog scene.
*/

#ifndef SPELL_ATTACK_SCREEN_H
#define SPELL_ATTACK_SCREEN_H

#include "screen.h"
#include <array>
#include <Shape/full_rectangle.h>

class Spell_Attack_Screen : public Screen {
private:
	std::array<int, 4> sp_inf = { {0, 0, 0, 0} };
	Full_Rectangle ranges[4];

	/*
	* Change range display of spell.
	*/
	void change_range();

	/*
	* Reset spell select.
	*/
	void reset_spell();

public:
	Spell_Attack_Screen();

	bool click_event_handler();

	void hover_event_handler();

	void draw();
};

#endif