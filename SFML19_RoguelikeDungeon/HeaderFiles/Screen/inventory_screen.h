#ifndef INVENTORY_SCREEN_H
#define INVENTORY_SCREEN_H

#include "screen.h"

/**
* Represents the inventory screen.
*/
class Inventory_Screen : public Screen {
public:
	Inventory_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void draw();

	void updateDraw();
};

#endif