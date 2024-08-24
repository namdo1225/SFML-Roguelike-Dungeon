/**
*
* File: load_screen.h
* Description: Contain the declaration of the Load_Screen class,
* which represents the load save screen.
*/

#ifndef LOAD_SCREEN_H
#define LOAD_SCREEN_H

#include "screen.h"

class Load_Screen : public Screen {
private:

public:
	Load_Screen();

	bool click_event_handler();

	void text_event_handler();
};

#endif