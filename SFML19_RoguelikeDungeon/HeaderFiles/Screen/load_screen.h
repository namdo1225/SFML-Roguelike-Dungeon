#include "screen.h"

#ifndef LOAD_SCREEN_H
#define LOAD_SCREEN_H

/**
* Represents the load save screen.
*/
class Load_Screen : public Screen {
public:
	Load_Screen();

	bool handleClickEvent();

	void handleTextEvent();
};

#endif