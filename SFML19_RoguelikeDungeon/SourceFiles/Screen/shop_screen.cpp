/**
*
* File: shop_screen.cpp
* Description: Contain the implementation of the Shop_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/shop_screen.h"
#include <format>
#include <Screen/screen.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>
#include <vector>

bool Shop_Screen::buy = true;
Stock Shop_Screen::stock = ItemStock;
unsigned int Shop_Screen::curSlots = 0;

Shop_Screen::Shop_Screen() : Screen(true, false) {
	update = true;

	setupTextbox("Buy", 100.f, 80.f, 80.f, 50.f, [this]() {
		buy = true;
		reset_select();
	});
	setupTextbox("Sell", 100.f, 150.f, 80.f, 50.f, [this]() {
		buy = stock == SpecialStock;
		reset_select();
	});
	setupTextbox("Items", 290.f, 80.f, 120.f, 50.f, [this]() {
		stock = ItemStock;
		reset_select();
	});
	setupTextbox("Spells", 290.f, 150.f, 120.f, 50.f, [this]() {
		stock = SpellStock;
		reset_select();
	});
	setupTextbox("Special", 450.f, 80.f, 130.f, 50.f, [this]() {
		stock = SpecialStock;
		buy = true;
		reset_select();
	});

	setupTextbox("Reset\nScroll", 10.f, 400.f, 120.f, 70.f, []() {
		viewSlots.reset(sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
	});
	setupTextbox("^", 10.f, 500.f, 50.f, 50.f, []() {
		viewSlots.move(sf::Vector2f(0.f, 10.f));
	});
	setupTextbox("v", 80.f, 500.f, 50.f, 50.f, []() {
		viewSlots.move(sf::Vector2f(0.f, -10.f));
	});
	setupTextbox("Clear\nMessage", 10.f, 600.f, 120.f, 70.f, [this]() {
		texts[3].setString("");
	});

	setup_helper(NULL, 50.f, -10.f, 650.f, 220.f, false);

	setup_helper("Shop", 360.f, 20.f, 36.f, NULL);

	setup_helper( "0/32 spells", 10.f, 250.f, 24.f, NULL);
	setup_helper(  "0/32 items", 10.f, 300.f, 24.f, NULL);
	setup_helper("", 50.f, 720.f, 24.f, NULL);

	update_draw();
}

bool Shop_Screen::click_event_handler() {
	update = true;
	if (mouse_in_button(ExitButton)) {
		viewSlots.reset(sf::FloatRect(0, 0, 1200, 800));
		reset_select();
		texts[3].setString("");
		switch_screen(ShopScreen, GameScreen, false, true);
		return true;
	}

	if (y > 220.f) {
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
}

void Shop_Screen::hover_event_handler() {
	update = true;
	for (unsigned int i = 0; i < curSlots; i++)
		hover_slot(i);
	hover_button(ConfirmButton);
}

void Shop_Screen::mouse_event_handler() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewSlots.move(sf::Vector2f(0.f, delta * 5));
	}
}

void Shop_Screen::draw() {
	window.draw(map_rects["background"]);

	bool selected = Game_Manager::inv_select || Game_Manager::spell_select || Game_Manager::special_select;

	window.setView(viewSlots);
	if (selected)
		window.draw(map_rects["inv_sp_cur_slot"]);

	for (unsigned int i = 0; i < curSlots; i++)
		window.draw(inv_sp_slots[i]);

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
	window.setView(viewUI);

	if (selected) {
		window.draw(map_rects["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_detail"]);
		window.draw(map_rects["confirm"]);
		window.draw(map_txts["confirm"]);
	}

	Screen::draw();

	window.draw(map_txts["inv_sp_gold"]);
	window.draw(map_txts["inv_sp_gold_amount"]);
}

void Shop_Screen::item_shop(bool buy) {
	unsigned int inv_select = Game_Manager::inv_select ? Game_Manager::inv_select->id : 0;
	std::vector<Item>& stocks = buy ? Game_Manager::item_stocks : Game_Manager::items;

	if (Game_Manager::inv_select && mouse_in_button(ConfirmButton)) {
		if (buy && Game_Manager::player.use_gold(Game_Manager::inv_select->buy)) {
			Game_Manager::add_item(inv_select);
			Game_Manager::reorganize_inv();
			texts[3].setString(std::format("Bought item {} for {}G.", Game_Manager::inv_select->name, Game_Manager::inv_select->buy));
		}
		else if (!buy) {
			Game_Manager::player.set_gold(Game_Manager::player.get_gold() + Game_Manager::inv_select->sell);
			texts[3].setString(std::format("Sold item {} for {}G.", Game_Manager::inv_select->name, Game_Manager::inv_select->sell));
			Game_Manager::delete_selected_itm();
		}
		Game_Manager::inv_select = NULL;
	}

	for (Item& item : stocks) {
		if (item.contains(slot_x, slot_y)) {
			int sx = item.getPos('x'), sy = item.getPos('y');
			// Selects an item
			if (!Game_Manager::inv_select) {
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
	unsigned int sp_select = Game_Manager::spell_select ? Game_Manager::spell_select->id : 0;
	std::vector<Spell>& stocks = buy ? Game_Manager::spell_stocks : Game_Manager::spells;

	if (Game_Manager::spell_select && mouse_in_button(ConfirmButton)) {
		if (buy && Game_Manager::player.use_gold(Game_Manager::spell_select->buy)) {
			Game_Manager::add_spell(sp_select);
			Game_Manager::reorganize_spell();
			texts[3].setString(std::format("Bought spell {} for {}G.", Game_Manager::spell_select->name, Game_Manager::spell_select->buy));
		}
		else if (!buy) {
			Game_Manager::player.set_gold(Game_Manager::player.get_gold() + Game_Manager::spell_select->sell);
			texts[3].setString(std::format("Sold spell {} for {}G.", Game_Manager::spell_select->name, Game_Manager::spell_select->sell));
			Game_Manager::deleted_selected_sp();
		}
		Game_Manager::spell_select = NULL;
	}

	for (Spell& spell : stocks) {
		if (spell.contains(slot_x, slot_y)) {
			int sx = spell.getPos('x'), sy = spell.getPos('y');
			// Selects a spell
			if (!Game_Manager::spell_select) {
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

void Shop_Screen::special_shop(bool buy) {
	if (!buy)
		return;

	unsigned int sp_select = Game_Manager::special_select ? Game_Manager::special_select->id : 0;
	std::vector<Special>& stocks = Game_Manager::special_stocks;

	if (Game_Manager::special_select && mouse_in_button(ConfirmButton)) {
		if (buy && Game_Manager::player.use_gold(Game_Manager::special_select->buy)) {
			Game_Manager::special_select->use();
			texts[3].setString(std::format("Bought {} for {}G.", Game_Manager::special_select->name, Game_Manager::special_select->buy));
		}
		Game_Manager::special_select = NULL;
	}

	for (Special& special : stocks) {
		if (special.contains(slot_x, slot_y)) {
			int sx = special.getPos('x'), sy = special.getPos('y');
			// Selects a special
			if (!Game_Manager::special_select) {
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
	texts[1].setString(std::format("{}/{} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));
	texts[2].setString(std::format("{}/{} items",
		Game_Manager::items.size(), Game_Manager::player.get_max_itm()));

	map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.get_gold()));

	curSlots = buy ? Spell::spells.size() : Game_Manager::spells.size();
	switch (stock) {
	case ItemStock:
		curSlots = buy ? Item::items.size() : Game_Manager::items.size();
		break;
	case SpecialStock:
		curSlots = Special::specials.size();
		break;
	};

	update = false;
}