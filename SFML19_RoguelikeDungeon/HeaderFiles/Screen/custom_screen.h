/**
*
* File: custom_screen.h
* Description: Contain the declaration of the Custom_Screen class, which represents a screen for players to add custom assets.
*/

#ifndef CUSTOM_SCREEN_H
#define CUSTOM_SCREEN_H

#include "screen.h"

class Custom_Screen : public Screen {
private:

public:
	Custom_Screen();

	void click_event_handler();

	void hover_event_handler();
};

#endif