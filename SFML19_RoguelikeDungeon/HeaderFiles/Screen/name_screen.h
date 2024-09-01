#include "screen.h"

#ifndef NAME_SCREEN_H
#define NAME_SCREEN_H

/**
* Represents the naming player screen.
*/
class Name_Screen : public Screen {
private:

public:
	Name_Screen();

	bool handleClickEvent();

	void handleKeyEvent();
};

#endif