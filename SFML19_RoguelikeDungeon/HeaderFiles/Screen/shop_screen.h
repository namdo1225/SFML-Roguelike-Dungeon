#ifndef SHOP_SCREEN_H
#define SHOP_SCREEN_H

#include "screen.h"
#include <Tool/tool.h>

/**
* Represents the shop creen.
*/
class Shop_Screen : public Screen {
private:
	static bool buy;
	static ToolEnum stock;
	static unsigned int curSlots;
	
	/*
	* Handle item shop.
	* 
	* Parameter:
	*	buy: true if buying. false if selling.
	*/
	void shopItem(bool buy);

	/*
	* Handle spell shop.
	*
	* Parameter:
	*	buy: true if buying. false if selling.
	*/
	void shopSpell(bool buy);

	/*
	* Handle special shop.
	*
	* Parameter:
	*	buy: true if buying. false if selling.
	*/
	void shopSpecial(bool buy);

	/*
	* Resets item, spell, and special select.
	*/
	static void resetSelect();

public:
	Shop_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void handleMouseEvent();

	void draw();

	void updateDraw();
};

#endif