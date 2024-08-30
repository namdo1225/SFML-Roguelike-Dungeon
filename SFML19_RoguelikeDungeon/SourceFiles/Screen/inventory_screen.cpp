/**
*
* File: inventory_screen.cpp
* Description: Contain the implementation of the Inventory_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/inventory_screen.h"
#include <format>
#include <Manager/audio_manager.h>
#include <Screen/screen.h>
#include <string>
#include <Tool/item.h>

Inventory_Screen::Inventory_Screen() : Screen(true, false) {
	update = true;
	setup_helper(NULL, 200.f, 140.f, 60.f, 60.f);
	setup_helper(NULL, 600.f, 140.f, 60.f, 60.f);

	setup_helper(      "Weapon", 200.f, 100.f, NULL, NULL);
	setup_helper(       "Armor", 600.f, 100.f, NULL, NULL);
	setup_helper(   "Inventory", 360.f,  20.f, 36.f, NULL);
	setup_helper( "0 / 0 items", 360.f, 150.f, 24.f, NULL);
	setup_helper( "", 50.f, 720.f, 24.f, NULL);
}

bool Inventory_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		Game_Manager::inv_select = NULL;
		switch_screen(InventoryScreen, GameScreen, false, true);
		texts[4].setString("");
		return true;
	}
	else if (mouse_in_button(UseButton) && Game_Manager::inv_select->type != Weapon && Game_Manager::inv_select->type != Armor) {
		texts[4].setString(std::format("You used a {}.", Game_Manager::inv_select->name));
		Game_Manager::item_use();
		return true;
	}
	else if (mouse_in_button(DiscardButton) &&
		Game_Manager::inv_select != Game_Manager::pl_weapon &&
		Game_Manager::inv_select != Game_Manager::pl_armor) {
		Game_Manager::delete_selected_itm();
		return true;
	}

	for (Item& item : Game_Manager::items) {
		if (item.contains(x, y)) {
			bool inv_select_contain = Game_Manager::inv_select && Game_Manager::inv_select->contains(x, y);
			unsigned int type = Game_Manager::inv_select ? Game_Manager::inv_select->type : -1;

			// Unselects
			if (type != -1 && inv_select_contain) {
				Game_Manager::inv_select = NULL;
				return true;
			}
			// Equips another weapon
			else if ((Game_Manager::inv_select == Game_Manager::pl_weapon && item.type == Weapon) ||
				(type == Weapon && mouse_in_helper(true, 0))) {
				Game_Manager::inv_select == Game_Manager::pl_weapon 
					? Game_Manager::equip_weapon(&item) 
					: Game_Manager::equip_weapon(Game_Manager::inv_select);

				Game_Manager::inv_select = NULL;
				return true;
			}
			// Equips another armor
			else if ((Game_Manager::inv_select == Game_Manager::pl_armor && item.type == Armor) ||
				(type == Armor && mouse_in_helper(true, 1))) {
				Game_Manager::inv_select == Game_Manager::pl_armor
					? Game_Manager::equip_armor(&item)
					: Game_Manager::equip_armor(Game_Manager::inv_select);

				Game_Manager::inv_select = NULL;
				return true;
			}
			// Swaps 2 item placement for non-weapon and non-armor slots
			else if (type != -1 && Game_Manager::inv_select != Game_Manager::pl_weapon &&
				Game_Manager::inv_select != Game_Manager::pl_armor &&
				!mouse_in_helper(true, 0) && !mouse_in_helper(true, 1)) {
				int i2x = Game_Manager::inv_select->getPos('x'), i2y = Game_Manager::inv_select->getPos('y');
				Game_Manager::inv_select->setPos(item.getPos('x'), item.getPos('y'));
				item.setPos(i2x, i2y);
				Game_Manager::inv_select = NULL;
				return true;
			}
			// Selects new item
			else if (type == -1) {
				int i1x = item.getPos('x'), i1y = item.getPos('y');
				Audio_Manager::play_sfx(4);
				Game_Manager::inv_select = &item;
				map_rects["inv_sp_cur_slot"].setPosition(i1x - 5, i1y - 5);
				map_txts["inv_sp_detail"].setString(item.desc.c_str());
				return true;
			}
		}
	}
	return false;
}

void Inventory_Screen::hover_event_handler() {
	hover_button(UseButton);
	hover_button(DiscardButton);

	for (unsigned int i = 0; i < Game_Manager::player.get_max_itm(); i++)
		hover_slot(i);

	hover_textbox(0, -1);
	hover_textbox(1, -1);
}

void Inventory_Screen::draw() {
	window.draw(map_rects["background"]);
	if (Game_Manager::inv_select)
		window.draw(map_rects["inv_sp_cur_slot"]);
	Screen::draw();

	for (unsigned int i = 0; i < Game_Manager::player.get_max_itm(); i++)
		window.draw(inv_sp_slots[i]);

	for (Item& itm : Game_Manager::items)
		itm.draw();

	if (Game_Manager::inv_select && Game_Manager::inv_select->type > Armor) {
		window.draw(map_rects["inv_sp_use"]);
		window.draw(map_txts["inv_sp_use"]);
	}

	if (Game_Manager::inv_select) {
		window.draw(map_rects["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_detail"]);
	}

	if (Game_Manager::inv_select != Game_Manager::pl_weapon &&
		Game_Manager::inv_select != Game_Manager::pl_armor &&
		Game_Manager::inv_select) {
		window.draw(map_rects["inv_sp_discard"]);
		window.draw(map_txts["inv_sp_discard"]);
	}

	window.draw(map_txts["inv_sp_gold"]);
	window.draw(map_txts["inv_sp_gold_amount"]);
}

void Inventory_Screen::update_draw() {
	texts[3].setString(std::format("{} / {} items",
		Game_Manager::items.size(),
		Game_Manager::player.get_max_itm()));

	map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.get_gold()));
}
