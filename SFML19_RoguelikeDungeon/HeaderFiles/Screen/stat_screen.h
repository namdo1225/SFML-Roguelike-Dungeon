#include "screen.h"

#ifndef STAT_SCREEN_H
#define STAT_SCREEN_H

/**
* Represents the new game's stat selection screen.
*/
class Stat_Screen : public Screen {
private:

public:
	Stat_Screen();

	void handleHoverEvent();

	void draw();

	void updateDraw();

	void handleKeyEvent();

	bool handleClickEvent();
};

#endif