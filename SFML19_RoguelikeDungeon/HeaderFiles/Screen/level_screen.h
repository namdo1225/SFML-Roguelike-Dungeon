#include "screen.h"
#include <array>
#include <stat.h>

#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

/**
* Represents the new game's stat selection screen.
*/
class Level_Screen : public Screen {
private:
	static std::array<long, StatConst::NUM_STATS> backup_stats;
	static unsigned int backup_pts;
	static bool reset;

public:
	Level_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void draw();

	void updateDraw();

	void handleKeyEvent();
};

#endif