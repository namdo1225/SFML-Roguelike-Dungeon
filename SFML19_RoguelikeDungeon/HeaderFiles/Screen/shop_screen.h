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

	/*
	* Handle item shop.
	* 
	* Parameter:
	*	buy: true if buying. false if selling.
	*/
	void item_shop(bool buy);

	/*
	* Handle spell shop.
	*
	* Parameter:
	*	buy: true if buying. false if selling.
	*/
	void spell_shop(bool buy);

	/*
	* Handle special shop.
	*
	* Parameter:
	*	buy: true if buying. false if selling.
	*/
	void special_shop(bool buy);

	/*
	* Resets item, spell, and special select.
	*/
	void reset_select();

public:
	Shop_Screen();

	virtual void click_event_handler();

	virtual void hover_event_handler();

	virtual void draw();

	virtual void update_draw();
};

#endif