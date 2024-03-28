/**
*
* File: stat_screen.h
* Description: Contain the declaration of the Stat_Screen class,
* which represents the new game's stat selection screen.
*/

#ifndef STAT_SCREEN_H
#define STAT_SCREEN_H

#include "screen.h"

class Stat_Screen : public Screen {
private:

public:
	Stat_Screen();

	virtual void click_event_handler();

	virtual void hover_event_handler();

	virtual void text_event_handler();

	virtual void draw();

	virtual void update_draw();
};

#endif