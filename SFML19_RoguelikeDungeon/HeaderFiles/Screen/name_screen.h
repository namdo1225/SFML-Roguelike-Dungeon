/**
*
* File: name_screen.h
* Description: Contain the declaration of the Name_Screen class, which represents the naming player screen.
*/

#ifndef NAME_SCREEN_H
#define NAME_SCREEN_H

#include "screen.h"

class Name_Screen : public Screen {
private:

public:
	Name_Screen();

	bool click_event_handler();

	void key_event_handler();
};

#endif