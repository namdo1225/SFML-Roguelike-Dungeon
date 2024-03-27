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
	bool show_help = false;

public:
	Menu_Screen();

	void click_event_handler();

	void hover_event_handler();

	virtual void draw();
};

#endif