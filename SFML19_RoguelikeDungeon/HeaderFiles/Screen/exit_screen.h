/**
*
* File: exit_screen.h
* Description: Contain the declaration of the Exit_Screen class, which represents the exit dialog scene.
*/

#ifndef EXIT_SCREEN_H
#define EXIT_SCREEN_H

#include "screen.h"

class Exit_Screen : public Screen {
private:

public:
	Exit_Screen();

	void click_event_handler();

	void hover_event_handler();
};

#endif