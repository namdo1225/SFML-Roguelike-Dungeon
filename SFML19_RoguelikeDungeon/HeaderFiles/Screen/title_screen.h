/**
*
* File: title_screen.h
* Description: Contain the declaration of the Title_Screen class, which represents the exit dialog scene.
*/

#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "screen.h"

class Title_Screen : public Screen {
private:

public:
	Title_Screen();

	void click_event_handler();

	void hover_event_handler();
};

#endif