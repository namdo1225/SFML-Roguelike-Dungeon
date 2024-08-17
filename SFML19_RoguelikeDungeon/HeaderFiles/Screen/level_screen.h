/**
*
* File: level_screen.h
* Description: Contain the declaration of the Level_Screen class,
* which represents the new game's stat selection screen.
*/

#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include "screen.h"
#include <array>

class Level_Screen : public Screen {
private:
	std::array<long, 8> backup_stats = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int backup_pts = 0;
	bool reset = true;

public:
	Level_Screen();

	virtual void click_event_handler();

	virtual void hover_event_handler();

	virtual void draw();

	virtual void update_draw();

	virtual void key_event_handler();
};

#endif