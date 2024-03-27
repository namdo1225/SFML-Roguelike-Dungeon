/**
*
* File: inventory_screen.cpp
* Description: Contain the implementation of the Inventory_Screen class.
*/

#include "Screen/inventory_screen.h"
#include "Manager/game_manager.h"
#include <format>

Inventory_Screen::Inventory_Screen() : Screen(4, 2, true, false) {
	update = true;
	setup_helper(true, 0, NULL, 200.f, 140.f, 60.f, 60.f);
	setup_helper(true, 1, NULL, 600.f, 140.f, 60.f, 60.f);

	setup_helper(false, 0,      "Weapon", 200.f, 100.f, NULL, NULL);
	setup_helper(false, 1,       "Armor", 600.f, 100.f, NULL, NULL);
	setup_helper(false, 2,   "Inventory", 360.f,  20.f, 36.f, NULL);
	setup_helper(false, 3, "0 / 0 items", 360.f, 150.f, 24.f, NULL);
}

void Inventory_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		Game_Manager::inv_select = Game_Manager::inv_draw_desc = Game_Manager::placeholder_item;
		switch_screen(InventoryScreen, GameScreen, false, true);
	}
	else if (mouse_in_button(UseButton) && Game_Manager::inv_select->get_type() == 0)
		Game_Manager::item_use();
	else if (mouse_in_button(DiscardButton) &&
		Game_Manager::inv_select->get_id() != 0 &&
		Game_Manager::inv_select != Game_Manager::pl_weapon &&
		Game_Manager::inv_select != Game_Manager::pl_weapon)
		Game_Manager::delete_selected_itm();

	for (std::shared_ptr<Item> item : Game_Manager::items) {
		if (item->contains(x, y)) {
			bool inv_select_contain = Game_Manager::inv_select->contains(x, y);
			unsigned int select_type = Game_Manager::inv_select->get_type();

			// Unselects
			if (select_type != 3 && inv_select_contain) {
				Game_Manager::inv_select = Game_Manager::inv_draw_desc =
					Game_Manager::placeholder_item;
				return;
			}
			// Equips another weapon
			else if ((Game_Manager::inv_select == Game_Manager::pl_weapon && item->get_type() == 1) ||
				(select_type == 1 && mouse_in_helper(true, 0))) {
				Game_Manager::inv_select == Game_Manager::pl_weapon 
					? Game_Manager::equip_weapon(item) 
					: Game_Manager::equip_weapon(Game_Manager::inv_select);

				Game_Manager::inv_select = Game_Manager::inv_draw_desc = Game_Manager::placeholder_item;
				return;
			}
			// Equips another armor
			else if ((Game_Manager::inv_select == Game_Manager::pl_armor && item->get_type() == 2) ||
				(select_type == 2 && mouse_in_helper(true, 1))) {
				Game_Manager::inv_select == Game_Manager::pl_armor
					? Game_Manager::equip_armor(item)
					: Game_Manager::equip_armor(Game_Manager::inv_select);

				Game_Manager::inv_select = Game_Manager::inv_draw_desc = Game_Manager::placeholder_item;
				return;
			}
			// Swaps 2 item placement for non-weapon and non-armor slots
			else if (select_type != 3 && Game_Manager::inv_select != Game_Manager::pl_weapon &&
				Game_Manager::inv_select != Game_Manager::pl_armor &&
				!mouse_in_helper(true, 0) && !mouse_in_helper(true, 1)) {
				int i2x = Game_Manager::inv_select->get_pos('x'), i2y = Game_Manager::inv_select->get_pos('y');
				Game_Manager::inv_select->set_pos(item->get_pos('x'), item->get_pos('y'));
				item->set_pos(i2x, i2y);
				Game_Manager::inv_select = Game_Manager::inv_draw_desc =
					Game_Manager::placeholder_item;
				return;
			}
			// Selects new item
			else if (select_type == 3) {
				int i1x = item->get_pos('x'), i1y = item->get_pos('y');
				Audio_Manager::play_sfx(4);
				Game_Manager::inv_draw_desc = Game_Manager::inv_select = item;
				inv_sp_cur_slot.setPosition(i1x - 5, i1y - 5);
				return;
			}
		}
	}
}

void Inventory_Screen::hover_event_handler() {
	hover_button(ExitButton);
	hover_button(UseButton);
	hover_button(DiscardButton);

	for (unsigned int i = 0; i < Game_Manager::player.get_max_itm(); i++)
		hover_slot(i);

	hover_textbox(0, -1);
	hover_textbox(1, -1);
}

void Inventory_Screen::draw() {
	window.draw(background);
	if (Game_Manager::inv_select->get_id())
		window.draw(inv_sp_cur_slot);
	Screen::draw();

	for (unsigned int i = 0; i < Game_Manager::player.get_max_itm(); i++)
		window.draw(inv_sp_slots[i]);

	for (std::shared_ptr<Item> itm : Game_Manager::items)
		itm->draw('t');

	if (Game_Manager::inv_select->get_type() == 0) {
		window.draw(inv_sp_use_rect);
		window.draw(inv_sp_use_txt);
	}

	window.draw(inv_sp_desc_rect);
	window.draw(inv_sp_desc_txt);

	if (Game_Manager::inv_draw_desc->get_id())
		Game_Manager::inv_draw_desc->draw('d');

	if (Game_Manager::inv_select != Game_Manager::pl_weapon &&
		Game_Manager::inv_select != Game_Manager::pl_armor &&
		Game_Manager::inv_select->get_id()) {
		window.draw(inv_sp_discard_rect);
		window.draw(inv_sp_discard_txt);
	}

	window.draw(inv_sp_gold_txt);
	window.draw(inv_sp_gold_amount_txt);
}

void Inventory_Screen::update_draw() {
	texts[3].setString(std::format("{} / {} items",
		Game_Manager::items.size(),
		Game_Manager::player.get_max_itm()));

	inv_sp_gold_amount_txt.setString(std::to_string(Game_Manager::player.get_gold()));
}
