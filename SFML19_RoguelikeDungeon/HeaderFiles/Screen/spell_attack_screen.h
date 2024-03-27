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

class Spell_Attack_Screen : public Screen {
private:
	std::array<int, 4> sp_inf = { {0, 0, 0, 0} };
	Full_Rectangle ranges[4];

	void change_range();

	void reset_spell();

public:
	Spell_Attack_Screen();

	void click_event_handler();

	void hover_event_handler();

	virtual void draw();
};

#endif