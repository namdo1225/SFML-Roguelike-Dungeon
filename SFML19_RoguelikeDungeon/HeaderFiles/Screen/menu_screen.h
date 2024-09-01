#include "screen.h"

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

/**
* Represents the menu dialog.
*/
class Menu_Screen : public Screen {
private:
	static bool help;

public:
	Menu_Screen();

	bool handleClickEvent();

	void draw();
};

#endif