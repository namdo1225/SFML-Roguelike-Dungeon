/**
*
* File: shop_screen.cpp
* Description: Contain the implementation of the Shop_Screen class.
* 
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

	textboxH("Buy", 100.f, 80.f, 80.f, 50.f, [this]() {
		buy = true;
		resetSelect();
	});
	textboxH("Sell", 100.f, 150.f, 80.f, 50.f, [this]() {
		buy = stock == SpecialStock;
		resetSelect();
	});
	textboxH("Items", 290.f, 80.f, 120.f, 50.f, [this]() {
		stock = ItemStock;
		resetSelect();
	});
	textboxH("Spells", 290.f, 150.f, 120.f, 50.f, [this]() {
		stock = SpellStock;
		resetSelect();
	});
	textboxH("Special", 450.f, 80.f, 130.f, 50.f, [this]() {
		stock = SpecialStock;
		buy = true;
		resetSelect();
	});

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
		texts[3].setString("");
	});

	textRectH(NULL, 50.f, -10.f, 650.f, 220.f, false);

	textRectH("Shop", 360.f, 20.f, 36.f, NULL);

	textRectH( "0/32 spells", 10.f, 250.f, 24.f, NULL);
	textRectH(  "0/32 items", 10.f, 300.f, 24.f, NULL);
	textRectH("", 50.f, 720.f, 24.f, NULL);

	updateDraw();
}

bool Shop_Screen::handleClickEvent() {
	update = true;
	if (mouseInButton(ExitButton)) {
		viewSlots.reset(sf::FloatRect(0, 0, 1200, 800));
		resetSelect();
		texts[3].setString("");
		switchScreen(ShopScreen, GameScreen, false, true);
		return true;
	}

	if (y > 220.f) {
		switch (stock) {
		case ItemStock:
			shopItem(buy);
			return true;
		case SpellStock:
			shopSpell(buy);
			return true;
		case SpecialStock:
			shopSpecial(buy);
			return true;
		}
	}
}

void Shop_Screen::handleHoverEvent() {
	update = true;
	for (unsigned int i = 0; i < curSlots; i++)
		hoverSlot(i);
	hoverButton(ConfirmButton);
}

void Shop_Screen::handleMouseEvent() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewSlots.move(sf::Vector2f(0.f, delta * 5));
	}
}

void Shop_Screen::draw() {
	window.draw(map_rects["background"]);

	bool selected = Game_Manager::selectedInv || Game_Manager::selectedSpell || Game_Manager::selectedSpecial;

	window.setView(viewSlots);
	if (selected)
		window.draw(map_rects["inv_sp_cur_slot"]);

	for (unsigned int i = 0; i < curSlots; i++)
		window.draw(inv_sp_slots[i]);

	if (buy && stock == ItemStock)
		for (Item item : Game_Manager::stockItem)
			item.draw();
	else if (!buy && stock == ItemStock)
		for (unsigned int i = 0; i < Game_Manager::items.size(); i++) {
			if (&Game_Manager::items[i] != Game_Manager::plArmor && &Game_Manager::items[i] != Game_Manager::plWeapon)
				Game_Manager::items[i].draw();
		}
	else if (buy && stock == SpellStock)
		for (Spell spell : Game_Manager::stockSpell)
			spell.draw();
	else if (!buy && stock == SpellStock)
		for (Spell spell : Game_Manager::spells)
			spell.draw();
	else if (buy && stock == SpecialStock)
		for (Special special : Game_Manager::stockSpecial)
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

void Shop_Screen::shopItem(bool buy) {
	unsigned int selectedInv = Game_Manager::selectedInv ? Game_Manager::selectedInv->getID() : 0;
	std::vector<Item>& stocks = buy ? Game_Manager::stockItem : Game_Manager::items;

	if (Game_Manager::selectedInv && mouseInButton(ConfirmButton)) {
		if (buy && Game_Manager::player.useGold(Game_Manager::selectedInv->getBuy())) {
			Game_Manager::addItem(selectedInv);
			Game_Manager::organizeInv();
			texts[3].setString(std::format("Bought item {} for {}G.", Game_Manager::selectedInv->getName(), Game_Manager::selectedInv->getBuy()));
		}
		else if (!buy) {
			Game_Manager::player.setGold(Game_Manager::player.getGold() + Game_Manager::selectedInv->getSell());
			texts[3].setString(std::format("Sold item {} for {}G.", Game_Manager::selectedInv->getName(), Game_Manager::selectedInv->getSell()));
			Game_Manager::delSelectedItem();
		}
		Game_Manager::selectedInv = NULL;
	}

	for (Item& item : stocks) {
		if (item.contains(slotX, slotY)) {
			int sx = item.getPos('x'), sy = item.getPos('y');
			// Selects an item
			if (!Game_Manager::selectedInv) {
				Game_Manager::selectedInv = &item;
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				map_txts["inv_sp_detail"].setString(item.getDesc());
				return;
			}
			// Unselects
			else if (Game_Manager::selectedInv == &item) {
				Game_Manager::selectedInv = NULL;
				return;
			}
		}
	}
}

void Shop_Screen::shopSpell(bool buy) {
	unsigned int sp_select = Game_Manager::selectedSpell ? Game_Manager::selectedSpell->getID() : 0;
	std::vector<Spell>& stocks = buy ? Game_Manager::stockSpell : Game_Manager::spells;

	if (Game_Manager::selectedSpell && mouseInButton(ConfirmButton)) {
		if (buy && Game_Manager::player.useGold(Game_Manager::selectedSpell->getBuy())) {
			Game_Manager::addSpell(sp_select);
			Game_Manager::organizeSpell();
			texts[3].setString(std::format("Bought spell {} for {}G.", Game_Manager::selectedSpell->getName(), Game_Manager::selectedSpell->getBuy()));
		}
		else if (!buy) {
			Game_Manager::player.setGold(Game_Manager::player.getGold() + Game_Manager::selectedSpell->getSell());
			texts[3].setString(std::format("Sold spell {} for {}G.", Game_Manager::selectedSpell->getName(), Game_Manager::selectedSpell->getSell()));
			Game_Manager::delSelectedSpell();
		}
		Game_Manager::selectedSpell = NULL;
	}

	for (Spell& spell : stocks) {
		if (spell.contains(slotX, slotY)) {
			int sx = spell.getPos('x'), sy = spell.getPos('y');
			// Selects a spell
			if (!Game_Manager::selectedSpell) {
				Game_Manager::selectedSpell = &spell;
				map_txts["inv_sp_detail"].setString(spell.getDesc());
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				return;
			}
			// Unselects
			else if (Game_Manager::selectedSpell == &spell) {
				Game_Manager::selectedSpell = NULL;
				return;
			}
		}
	}
}

void Shop_Screen::shopSpecial(bool buy) {
	if (!buy)
		return;

	unsigned int sp_select = Game_Manager::selectedSpecial ? Game_Manager::selectedSpecial->getID() : 0;
	std::vector<Special>& stocks = Game_Manager::stockSpecial;

	if (Game_Manager::selectedSpecial && mouseInButton(ConfirmButton)) {
		if (buy && Game_Manager::player.useGold(Game_Manager::selectedSpecial->getBuy())) {
			Game_Manager::selectedSpecial->use();
			texts[3].setString(std::format("Bought {} for {}G.", Game_Manager::selectedSpecial->getName(), Game_Manager::selectedSpecial->getBuy()));
		}
		Game_Manager::selectedSpecial = NULL;
	}

	for (Special& special : stocks) {
		if (special.contains(slotX, slotY)) {
			int sx = special.getPos('x'), sy = special.getPos('y');
			// Selects a special
			if (!Game_Manager::selectedSpecial) {
				Game_Manager::selectedSpecial = &special;
				map_txts["inv_sp_detail"].setString(special.getDesc());
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				return;
			}
			// Unselects
			else if (Game_Manager::selectedSpecial == &special) {
				Game_Manager::selectedSpecial = NULL;
				return;
			}
		}
	}
}

void Shop_Screen::resetSelect() {
	Game_Manager::selectedSpell = NULL;
	Game_Manager::selectedInv = NULL;
	Game_Manager::selectedSpecial = NULL;
}

void Shop_Screen::updateDraw() {
	texts[1].setString(std::format("{}/{} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));
	texts[2].setString(std::format("{}/{} items",
		Game_Manager::items.size(), Game_Manager::player.getMaxItems()));

	map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.getGold()));

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