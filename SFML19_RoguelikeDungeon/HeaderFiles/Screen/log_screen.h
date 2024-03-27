/**
*
* File: log_screen.h
* Description: Contain the declaration of the Load_Screen class,
* which represents the load save screen.
*/

#ifndef LOG_SCREEN_H
#define LOG_SCREEN_H

#include "screen.h"

class Log_Screen : public Screen {
private:
	const static unsigned int MAX_LOGS = 50;

public:
	Log_Screen();

	void click_event_handler();

	void hover_event_handler();

	virtual void draw();

	void mouse_event_handler();
};

#endif