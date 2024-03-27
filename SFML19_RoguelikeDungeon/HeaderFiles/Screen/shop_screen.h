/**
*
* File: shop_screen.h
* Description: Contain the declaration of the Shop_Screen class, which represents the shop creen.
*/

#ifndef SHOP_SCREEN_H
#define SHOP_SCREEN_H

#include "screen.h"

enum Stock {ItemStock, SpellStock, SpecialStock};

class Shop_Screen : public Screen {
private:
	static bool buy;
	static Stock stock;

	void item_shop(bool buy);

	void spell_shop(bool buy);

	void special_shop(bool buy);

	void reset_select();
public:
	Shop_Screen();

	void click_event_handler();

	void hover_event_handler();

	virtual void draw();

	virtual void update_draw();
};

#endif