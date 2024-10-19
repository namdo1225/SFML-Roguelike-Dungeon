#include "screen.h"

#ifndef SPELL_SCREEN_H
#define SPELL_SCREEN_H

/**
* Represents the spell screen.
*/
class Spell_Screen : public Screen {
public:
	Spell_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void handleMouseEvent();

	void draw();

	void updateDraw();
};

#endif