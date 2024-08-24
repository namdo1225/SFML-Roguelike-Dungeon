/**
*
* File: inventory_screen.h
* Description: Contain the declaration of the Inventory_Screen class, which represents the inventory screen.
*/

#ifndef INVENTORY_SCREEN_H
#define INVENTORY_SCREEN_H

#include "screen.h"

class Inventory_Screen : public Screen {
private:

public:
	Inventory_Screen();

	virtual bool click_event_handler();

	virtual void hover_event_handler();

	virtual void draw();

	virtual void update_draw();
};

#endif