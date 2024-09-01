#ifndef LOG_SCREEN_H
#define LOG_SCREEN_H

#include "screen.h"

/**
* Represents the log screen.
*/
class Log_Screen : public Screen {
public:
	Log_Screen();

	bool handleClickEvent();

	void draw();

	void handleMouseEvent();
};

#endif