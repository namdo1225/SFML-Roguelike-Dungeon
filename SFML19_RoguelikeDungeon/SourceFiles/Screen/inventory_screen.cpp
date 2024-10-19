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
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <Tool/item.h>
#include <Tool/tool.h>

Item* Inventory_Screen::selected = NULL;

void Inventory_Screen::equip(unsigned int i, ItemType type) {
	selected == (type == Weapon ? &Game_Manager::plWeapon : &Game_Manager::plArmor)
		? Game_Manager::equipItem(i, type)
		: Game_Manager::equipItem(Game_Manager::selectedInv, type);

	Game_Manager::selectedInv = SelectNone;
}

Inventory_Screen::Inventory_Screen() : Screen(true, false) {
	update = true;

	textRectH(      "Weapon", 200.f, 100.f, NULL, NULL);
	textRectH(       "Armor", 600.f, 100.f, NULL, NULL);
	textRectH(   "Inventory", 360.f,  20.f, 36.f, NULL);
	textRectH( "0 / 0 items", 360.f, 100.f, 24.f, NULL);
	textRectH( "", 50.f, 720.f, 24.f, NULL);

	textboxH("Reset\nScroll", 10.f, 400.f, 120.f, 70.f, []() {
		viewSlots.reset(sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
		});
	textboxH("^", 10.f, 500.f, 50.f, 50.f, []() {
		viewSlots.move(sf::Vector2f(0.f, 10.f));
		});
	textboxH("v", 80.f, 500.f, 50.f, 50.f, []() {
		viewSlots.move(sf::Vector2f(0.f, -10.f));
		});
	textboxH("Clear\nMessage", 10.f, 600.f, 120.f, 70.f, [this]() {
		texts[4].setString("");
		});

	textRectH(NULL, 50.f, -10.f, 650.f, 140.f, false);
}

bool Inventory_Screen::handleClickEvent() {
	selected = Game_Manager::getSelectedItem();

	if (mouseInButton(ExitButton)) {
		Game_Manager::selectedInv = SelectNone;
		switchScreen(InventoryScreen, GameScreen, false, true);
		texts[4].setString("");
		return true;
	}
	else if (selected) {
		if (mouseInButton(UseButton)) {
			if (selected->getType() == Weapon || selected->getType() == Armor)
				equip(Game_Manager::selectedInv, (ItemType)selected->getType());
			else {
				texts[4].setString(std::format("You used a {}.", selected->getName()));
				Game_Manager::useItem();
			}
			return true;
		}
		else if (mouseInButton(DiscardButton) &&
			selected != &Game_Manager::plWeapon &&
			selected != &Game_Manager::plArmor) {
			Game_Manager::delSelectedTool(ItemTool);
			return true;
		}
	}

	for (unsigned int i = 0; i < Game_Manager::items.size() + 2; i++) {
		Item* item = NULL;
		if (i == Game_Manager::items.size())
			item = &Game_Manager::plWeapon;
		else if (i == Game_Manager::items.size() + 1)
			item = &Game_Manager::plArmor;
		else
			item = &Game_Manager::items[i];

		if (item->contains(slotX, slotY) && y > 140.f) {
			bool inv_select_contain = selected && selected->contains(slotX, slotY);
			unsigned int type = selected ? selected->getType() : SelectNone;

			// Unselects
			if (selected && inv_select_contain) {
				Game_Manager::selectedInv = SelectNone;
				return true;
			}
			// Equips another weapon
			else if (selected && ((selected == &Game_Manager::plWeapon && item->getType() == Weapon) ||
				(selected->getType() == Weapon && item == &Game_Manager::plWeapon)) && selected != item) {
				equip(i, Weapon);
				return true;
			}
			// Equips another armor
			else if (selected && ((selected == &Game_Manager::plArmor && item->getType() == Armor) ||
				(selected->getType() == Armor && item == &Game_Manager::plArmor)) && selected != item) {
				equip(i, Armor);
				return true;
			}
			// Swaps 2 item placement for non-weapon and non-armor slots
			else if (selected && selected != &Game_Manager::plWeapon &&
				selected != &Game_Manager::plArmor && item != selected &&
				item != &Game_Manager::plWeapon && item != &Game_Manager::plArmor) {
				int i2x = selected->getPos('x'), i2y = selected->getPos('y');
				selected->setPos(item->getPos('x'), item->getPos('y'));
				item->setPos(i2x, i2y);
				Game_Manager::selectedInv = SelectNone;
				return true;
			}
			// Selects new item
			else if (!selected) {
				int i1x = item->getPos('x'), i1y = item->getPos('y');
				Audio_Manager::playSFX(4);

				if (item == &Game_Manager::plWeapon)
					Game_Manager::selectedInv = SelectWeapon;
				else if (item == &Game_Manager::plArmor)
					Game_Manager::selectedInv = SelectArmor;
				else
					Game_Manager::selectedInv = i;

				map_rects["inv_sp_cur_slot"].setPosition(i1x - 5, i1y - 5);
				map_txts["inv_sp_detail"].setString(item->getDesc());
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
}

void Inventory_Screen::handleMouseEvent() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewSlots.move(sf::Vector2f(0.f, delta * 5));
	}
}

void Inventory_Screen::draw() {
	Item* selected = Game_Manager::getSelectedItem();

	window.draw(map_rects["background"]);

	window.setView(viewSlots);
	if (selected)
		window.draw(map_rects["inv_sp_cur_slot"]);

	for (unsigned int i = 0; i < Game_Manager::player.getMaxItems(); i++)
		window.draw(inv_sp_slots[i]);

	for (Item& itm : Game_Manager::items)
		itm.draw();

	Game_Manager::plWeapon.draw();
	Game_Manager::plArmor.draw();

	window.setView(viewUI);

	Screen::draw();

	if (selected) {
		window.draw(map_rects["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_desc"]);
		window.draw(map_txts["inv_sp_detail"]);

		if (selected != &Game_Manager::plWeapon &&
			selected != &Game_Manager::plArmor) {
			window.draw(map_rects["inv_sp_use"]);
			window.draw(map_txts["inv_sp_use"]);
			window.draw(map_rects["inv_sp_discard"]);
			window.draw(map_txts["inv_sp_discard"]);
		}

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