/**
*
* File: menu_screen.h
* Description: Contain the declaration of the Menu_Screen class, which represents the menu dialog.
*/

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "screen.h"

class Menu_Screen : public Screen {
private:
	static bool help;

public:
	Menu_Screen();

	bool click_event_handler();

	void draw();
};

#endif