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
	textRectH(NULL, 200.f, 140.f, 60.f, 60.f);
	textRectH(NULL, 600.f, 140.f, 60.f, 60.f);

	textRectH(      "Weapon", 200.f, 100.f, NULL, NULL);
	textRectH(       "Armor", 600.f, 100.f, NULL, NULL);
	textRectH(   "Inventory", 360.f,  20.f, 36.f, NULL);
	textRectH( "0 / 0 items", 360.f, 150.f, 24.f, NULL);
	textRectH( "", 50.f, 720.f, 24.f, NULL);
}

bool Inventory_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		Game_Manager::selectedInv = NULL;
		switchScreen(InventoryScreen, GameScreen, false, true);
		texts[4].setString("");
		return true;
	}
	else if (mouseInButton(UseButton) && Game_Manager::selectedInv->getType() != Weapon && Game_Manager::selectedInv->getType() != Armor) {
		texts[4].setString(std::format("You used a {}.", Game_Manager::selectedInv->getName()));
		Game_Manager::useItem();
		return true;
	}
	else if (mouseInButton(DiscardButton) &&
		Game_Manager::selectedInv != Game_Manager::plWeapon &&
		Game_Manager::selectedInv != Game_Manager::plArmor) {
		Game_Manager::delSelectedItem();
		return true;
	}

	for (Item& item : Game_Manager::items) {
		if (item.contains(x, y)) {
			bool inv_select_contain = Game_Manager::selectedInv && Game_Manager::selectedInv->contains(x, y);
			unsigned int type = Game_Manager::selectedInv ? Game_Manager::selectedInv->getType() : -1;

			// Unselects
			if (type != -1 && inv_select_contain) {
				Game_Manager::selectedInv = NULL;
				return true;
			}
			// Equips another weapon
			else if ((Game_Manager::selectedInv == Game_Manager::plWeapon && item.getType() == Weapon) ||
				(type == Weapon && mouseInH(true, 0))) {
				Game_Manager::selectedInv == Game_Manager::plWeapon 
					? Game_Manager::equipWeapon(&item) 
					: Game_Manager::equipWeapon(Game_Manager::selectedInv);

				Game_Manager::selectedInv = NULL;
				return true;
			}
			// Equips another armor
			else if ((Game_Manager::selectedInv == Game_Manager::plArmor && item.getType() == Armor) ||
				(type == Armor && mouseInH(true, 1))) {
				Game_Manager::selectedInv == Game_Manager::plArmor
					? Game_Manager::equipArmor(&item)
					: Game_Manager::equipArmor(Game_Manager::selectedInv);

				Game_Manager::selectedInv = NULL;
				return true;
			}
			// Swaps 2 item placement for non-weapon and non-armor slots
			else if (type != -1 && Game_Manager::selectedInv != Game_Manager::plWeapon &&
				Game_Manager::selectedInv != Game_Manager::plArmor &&
				!mouseInH(true, 0) && !mouseInH(true, 1)) {
				int i2x = Game_Manager::selectedInv->getPos('x'), i2y = Game_Manager::selectedInv->getPos('y');
				Game_Manager::selectedInv->setPos(item.getPos('x'), item.getPos('y'));
				item.setPos(i2x, i2y);
				Game_Manager::selectedInv = NULL;
				return true;
			}
			// Selects new item
			else if (type == -1) {
				int i1x = item.getPos('x'), i1y = item.getPos('y');
				Audio_Manager::playSFX(4);
				Game_Manager::selectedInv = &item;
				map_rects["inv_sp_cur_slot"].setPosition(i1x - 5, i1y - 5);
				map_txts["inv_sp_detail"].setString(item.getDesc());
				return true;
			}
		}
	}
	return false;
}

void Inventory_Screen::handleHoverEvent() {
	hoverButton(UseButton);
	hoverButton(DiscardButton);

	for (unsigned int i = 0; i < Game_Manager::player.getMaxItems(); i++)
		hoverSlot(i);

	hoverTextRect(0, -1);
	hoverTextRect(1, -1);
}

void Inventory_Screen::draw() {
	window.draw(map_rects["background"]);
	if (Game_Manager::selectedInv)
		window.draw(map_rects["inv_sp_cur_slot"]);
	Screen::draw();

	for (unsigned int i = 0; i < Game_Manager::player.getMaxItems(); i++)
		window.draw(inv_sp_slots[i]);

	for (Item& itm : Game_Manager::items)
		itm.draw();

	if (Game_Manager::selectedInv && Game_Manager::selectedInv->getType() > Armor) {
		window.draw(map_rects["inv_sp_use"]);
		window.draw(map_txts["inv_sp_use"]);
	}

	if (Game_Manager::selectedInv) {
		window.draw(map_rects["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_detail"]);
	}

	if (Game_Manager::selectedInv != Game_Manager::plWeapon &&
		Game_Manager::selectedInv != Game_Manager::plArmor &&
		Game_Manager::selectedInv) {
		window.draw(map_rects["inv_sp_discard"]);
		window.draw(map_txts["inv_sp_discard"]);
	}

	window.draw(map_txts["inv_sp_gold"]);
	window.draw(map_txts["inv_sp_gold_amount"]);
}

void Inventory_Screen::updateDraw() {
	texts[3].setString(std::format("{} / {} items",
		Game_Manager::items.size(),
		Game_Manager::player.getMaxItems()));

	map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.getGold()));
}
