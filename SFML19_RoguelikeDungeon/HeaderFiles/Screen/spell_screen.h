/**
*
* File: spell_screen.h
* Description: Contain the declaration of the Spell_Screen class, which represents the spell screen.
*/

#ifndef SPELL_SCREEN_H
#define SPELL_SCREEN_H

#include "screen.h"

class Spell_Screen : public Screen {
private:

public:
	Spell_Screen();

	void click_event_handler();

	void hover_event_handler();

	virtual void draw();

	virtual void update_draw();
};

#endif