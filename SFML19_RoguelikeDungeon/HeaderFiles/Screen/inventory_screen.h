#ifndef INVENTORY_SCREEN_H
#define INVENTORY_SCREEN_H

#include "screen.h"
#include <Tool/item.h>

/**
* Represents the inventory screen.
*/
class Inventory_Screen : public Screen {
private:
	static Item* selected;

	/*
	* Equips weapon or armor.
	* 
	* Parameter:
	*	i: the index to the item slot.
	*	type: Weapon or Armor.
	*/
	void equip(unsigned int i, ItemType type);

public:
	Inventory_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void handleMouseEvent();

	void draw();

	void updateDraw();
};

#endif