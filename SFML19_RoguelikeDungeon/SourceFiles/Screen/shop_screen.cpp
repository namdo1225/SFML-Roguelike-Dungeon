/**
*
* File: shop_screen.cpp
* Description: Contain the implementation of the Shop_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/shop_screen.h"
#include <format>
#include <Screen/screen.h>
#include <Shape/full_rectangle.h>
#include <string>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>
#include <vector>

bool Shop_Screen::buy = true;
Stock Shop_Screen::stock = ItemStock;

Shop_Screen::Shop_Screen() : Screen(true, false, false) {
	update = true;

	setup_helper(NULL, 100.f,  80.f,  80.f, 50.f);
	setup_helper(NULL, 100.f, 150.f,  80.f, 50.f);
	setup_helper(NULL, 290.f,  80.f, 120.f, 50.f);
	setup_helper(NULL, 290.f, 150.f, 120.f, 50.f);
	setup_helper(NULL, 450.f,  80.f, 130.f, 50.f);

	setup_helper(NULL,  113.f,  125.f,  50.f,  0.f);
	setup_helper(NULL,  300.f,  125.f, 100.f,  0.f);

	setup_helper("Shop", 360.f, 20.f, 36.f, NULL);

	setup_helper(    "Buy", 110.f,  90.f, 30.f, NULL);
	setup_helper(   "Sell", 110.f, 160.f, 30.f, NULL);
	setup_helper(  "Items", 300.f,  90.f, 30.f, NULL);
	setup_helper( "Spells", 300.f, 160.f, 30.f, NULL);
	setup_helper("Special", 460.f,  90.f, 30.f, NULL);

	setup_helper( "0 / 32 spells", 460.f, 150.f, 24.f, NULL);
	setup_helper(  "0 / 32 items", 460.f, 190.f, 24.f, NULL);
	setup_helper("", 50.f, 720.f, 24.f, NULL);
}

bool Shop_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		reset_select();
		texts[8].setString("");
		switch_screen(ShopScreen, GameScreen, false, true);
		return true;
	}
	// buy button
	else if (mouse_in_helper(true, 0)) {
		rects[5].setPosition(113.f, 125.f);
		buy = true;
		reset_select();
		return true;
	}
	// sell button
	else if (mouse_in_helper(true, 1)) {
		rects[5].setPosition(113.f, 195.f);
		buy = false;
		if (stock == SpecialStock) {
			buy = true;
			rects[5].setPosition(113.f, 125.f);
		}
		reset_select();
		return true;
	}
	// items button
	else if (mouse_in_helper(true, 2)) {
		rects[6].setPosition(300.f, 125.f);
		stock = ItemStock;
		reset_select();
		return true;
	}
	// spells button
	else if (mouse_in_helper(true, 3)) {
		rects[6].setPosition(300.f, 195.f);
		stock = SpellStock;
		reset_select();
		return true;
	}
	// special button
	else if (mouse_in_helper(true, 4)) {
		rects[6].setPosition(460.f, 125.f);
		stock = SpecialStock;
		if (!buy) {
			buy = true;
			rects[5].setPosition(113.f, 125.f);
		}
		reset_select();
		return true;
	}

	switch (stock) {
	case ItemStock:
		item_shop(buy);
		return true;
	case SpellStock:
		spell_shop(buy);
		return true;
	case SpecialStock:
		special_shop(buy);
		return true;
	}
}

void Shop_Screen::hover_event_handler() {
	for (unsigned int i = 0; i < MAX_INV_SPELL_SLOTS; i++)
		hover_slot(i);

	for (unsigned int i = 0; i < 5; i++)
		hover_textbox(i, i + 1);
}

void Shop_Screen::draw() {
	window.draw(map_rects["background"]);

	bool selected = Game_Manager::inv_select || Game_Manager::spell_select || Game_Manager::special_select;
	window.draw(map_rects["inv_sp_desc"]);
	window.draw(map_txts["inv_sp_desc"]);


	if (selected) {
		window.draw(map_rects["inv_sp_cur_slot"]);
		window.draw(map_txts["inv_sp_detail"]);
		window.draw(map_rects["confirm"]);
		window.draw(map_txts["confirm"]);
	}

	Screen::draw();

	for (Full_Rectangle& rect : inv_sp_slots)
		window.draw(rect);

	if (buy && stock == ItemStock)
		for (Item item : Game_Manager::item_stocks)
			item.draw();
	else if (!buy && stock == ItemStock)
		for (unsigned int i = 0; i < Game_Manager::items.size(); i++) {
			if (&Game_Manager::items[i] != Game_Manager::pl_armor && &Game_Manager::items[i] != Game_Manager::pl_weapon)
				Game_Manager::items[i].draw();
		}
	else if (buy && stock == SpellStock)
		for (Spell spell : Game_Manager::spell_stocks)
			spell.draw();
	else if (!buy && stock == SpellStock)
		for (Spell spell : Game_Manager::spells)
			spell.draw();
	else if (buy && stock == SpecialStock)
		for (Special special : Game_Manager::special_stocks)
			special.draw();		

	window.draw(map_txts["inv_sp_gold"]);
	window.draw(map_txts["inv_sp_gold_amount"]);
}

void Shop_Screen::item_shop(bool buy) {
	unsigned int inv_select = Game_Manager::inv_select->id;
	std::vector<Item>& stocks = buy ? Game_Manager::item_stocks : Game_Manager::items;

	if (inv_select && mouse_in_button(ConfirmButton)) {
		if (buy && Game_Manager::player.use_gold(Game_Manager::inv_select->buy)) {
			Game_Manager::add_item(inv_select);
			Game_Manager::reorganize_inv();
			texts[8].setString(std::format("Bought item {} for {}G.", Game_Manager::inv_select->name, Game_Manager::inv_select->buy));
		}
		else if (!buy) {
			Game_Manager::player.set_gold(Game_Manager::player.get_gold() + Game_Manager::inv_select->sell);
			Game_Manager::delete_selected_itm();
			texts[8].setString(std::format("Sold item {} for {}G.", Game_Manager::inv_select->name, Game_Manager::inv_select->sell));
		}
		Game_Manager::inv_select = NULL;
	}

	for (Item item : stocks) {
		if (item.contains(x, y)) {
			int sx = item.getPos('x'), sy = item.getPos('y');
			// Selects another item
			if (inv_select != 0 && &item != Game_Manager::inv_select) {
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				Game_Manager::inv_select = &item;
				map_txts["inv_sp_detail"].setString(item.desc);
				return;
			}
			// Selects an item
			else if (inv_select == 0) {
				Game_Manager::inv_select = &item;
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				map_txts["inv_sp_detail"].setString(item.desc);
				return;
			}
			// Unselects
			else if (Game_Manager::inv_select == &item) {
				Game_Manager::inv_select = NULL;
				return;
			}
		}
	}
}

void Shop_Screen::spell_shop(bool buy) {
	unsigned int sp_select = Game_Manager::spell_select->id;
	std::vector<Spell>& stocks = buy ? Game_Manager::spell_stocks : Game_Manager::spells;

	if (sp_select && mouse_in_button(ConfirmButton)) {
		if (buy && Game_Manager::player.use_gold(Game_Manager::spell_select->buy)) {
			Game_Manager::add_spell(sp_select);
			Game_Manager::reorganize_spell();
			texts[8].setString(std::format("Bought spell {} for {}G.", Game_Manager::spell_select->name, Game_Manager::spell_select->buy));
		}
		else if (!buy) {
			Game_Manager::player.set_gold(Game_Manager::player.get_gold() + Game_Manager::spell_select->sell);
			Game_Manager::deleted_selected_sp();
			texts[8].setString(std::format("Sold spell {} for {}G.", Game_Manager::spell_select->name, Game_Manager::spell_select->sell));
		}
		Game_Manager::spell_select = NULL;
	}

	for (Spell spell : stocks) {
		if (spell.contains(x, y)) {
			int sx = spell.getPos('x'), sy = spell.getPos('y');
			// Selects another spell
			if (sp_select && &spell != Game_Manager::spell_select) {
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				map_txts["inv_sp_detail"].setString(spell.desc);
				Game_Manager::spell_select = &spell;
				return;
			}
			// Selects a spell
			else if (!sp_select) {
				Game_Manager::spell_select = &spell;
				map_txts["inv_sp_detail"].setString(spell.desc);
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				return;
			}
			// Unselects
			else if (Game_Manager::spell_select == &spell) {
				Game_Manager::spell_select = NULL;
				return;
			}
		}
	}
}

void Shop_Screen::special_shop(bool buy)
{
	if (!buy)
		return;

	unsigned int sp_select = Game_Manager::special_select->id;
	std::vector<Special>& stocks = Game_Manager::special_stocks;

	if (sp_select && mouse_in_button(ConfirmButton)) {
		if (buy && Game_Manager::player.use_gold(Game_Manager::special_select->buy)) {
			Game_Manager::special_select->use();
			texts[8].setString(std::format("Bought {} for {}G.", Game_Manager::special_select->name, Game_Manager::special_select->buy));
		}
		Game_Manager::special_select = NULL;
	}

	for (Special special : stocks) {
		if (special.contains(x, y)) {
			int sx = special.getPos('x'), sy = special.getPos('y');
			// Selects another special
			if (sp_select && &special != Game_Manager::special_select) {
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				Game_Manager::special_select = &special;
				map_txts["inv_sp_detail"].setString(special.desc);
				return;
			}
			// Selects a special
			else if (!sp_select) {
				Game_Manager::special_select = &special;
				map_txts["inv_sp_detail"].setString(special.desc);
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				return;
			}
			// Unselects
			else if (Game_Manager::special_select == &special) {
				Game_Manager::special_select = NULL;
				return;
			}
		}
	}
}

void Shop_Screen::reset_select() {
	Game_Manager::spell_select = NULL;
	Game_Manager::inv_select = NULL;
	Game_Manager::special_select = NULL;
}

void Shop_Screen::update_draw() {
	texts[6].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));
	texts[7].setString(std::format("{} / {} items",
		Game_Manager::items.size(), Game_Manager::player.get_max_itm()));

	map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.get_gold()));
}