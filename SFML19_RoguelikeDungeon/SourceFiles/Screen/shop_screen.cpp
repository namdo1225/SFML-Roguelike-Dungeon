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
#include <Tool/tool.h>
#include <vector>

bool Shop_Screen::buy = true;
ToolEnum Shop_Screen::stock = ItemTool;
unsigned int Shop_Screen::curSlots = 0;

Shop_Screen::Shop_Screen() : Screen(true, false) {
	update = true;

	textboxH("Buy", 100.f, 80.f, 80.f, 50.f, [this]() {
		buy = true;
		resetSelect();
	});
	textboxH("Sell", 100.f, 150.f, 80.f, 50.f, [this]() {
		buy = stock == SpecialTool;
		resetSelect();
	});
	textboxH("Items", 290.f, 80.f, 120.f, 50.f, [this]() {
		stock = ItemTool;
		resetSelect();
	});
	textboxH("Spells", 290.f, 150.f, 120.f, 50.f, [this]() {
		stock = SpellTool;
		resetSelect();
	});
	textboxH("Special", 450.f, 80.f, 130.f, 50.f, [this]() {
		stock = SpecialTool;
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
		texts[3].setString("");
		resetSelect();
		switchScreen(ShopScreen, GameScreen, false, true);
		return true;
	}

	if (y > 220.f) {
		switch (stock) {
		case ItemTool:
			shopItem(buy);
			return true;
		case SpellTool:
			shopSpell(buy);
			return true;
		case SpecialTool:
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

	bool selected = Game_Manager::selectedInv != SelectNone ||
		Game_Manager::selectedSpell != SelectNone ||
		Game_Manager::selectedSpecial != SelectNone;

	window.setView(viewSlots);
	if (selected)
		window.draw(map_rects["inv_sp_cur_slot"]);

	for (unsigned int i = 0; i < curSlots; i++)
		window.draw(inv_sp_slots[i]);

	if (buy && stock == ItemTool)
		for (Item item : Game_Manager::stockItem)
			item.draw();
	else if (!buy && stock == ItemTool)
		for (unsigned int i = 0; i < Game_Manager::items.size(); i++)
				Game_Manager::items[i].draw();
	else if (buy && stock == SpellTool)
		for (Spell spell : Game_Manager::stockSpell)
			spell.draw();
	else if (!buy && stock == SpellTool)
		for (Spell spell : Game_Manager::spells)
			spell.draw();
	else if (buy && stock == SpecialTool)
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
	std::vector<Item>& stocks = buy ? Game_Manager::stockItem : Game_Manager::items;
	Item* selected = Game_Manager::getSelectedItem(stocks);

	if (selected && mouseInButton(ConfirmButton)) {
		if (buy && Game_Manager::player.useGold(selected->getBuy())) {
			Game_Manager::addTool(selected->getID(), ItemTool);
			Game_Manager::organizeTool(ItemTool);
			texts[3].setString(std::format("Bought item {} for {}G.", selected->getName(), selected->getBuy()));
		}
		else if (!buy) {
			Game_Manager::player.setGold(Game_Manager::player.getGold() + selected->getSell());
			texts[3].setString(std::format("Sold item {} for {}G.", selected->getName(), selected->getSell()));
			Game_Manager::delSelectedTool(ItemTool);
		}
		Game_Manager::selectedInv = SelectNone;
	}

	for (unsigned int i = 0; i < stocks.size(); i++) {
		Item* item = &stocks[i];
		if (item->contains(slotX, slotY)) {
			// Selects an item
			int sx = item->getPos('x'), sy = item->getPos('y');
			Game_Manager::selectedInv = i;
			map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
			map_txts["inv_sp_detail"].setString(item->getDesc());
			return;
		}
	}
}

void Shop_Screen::shopSpell(bool buy) {
	std::vector<Spell>& stocks = buy ? Game_Manager::stockSpell : Game_Manager::spells;
	Spell* select = Game_Manager::selectedSpell != SelectNone ? &stocks[Game_Manager::selectedSpell] : NULL;

	if (select && mouseInButton(ConfirmButton)) {
		if (buy && Game_Manager::player.useGold(select->getBuy())) {
			Game_Manager::addTool(select->getID(), SpellTool);
			Game_Manager::organizeTool(SpellTool);
			texts[3].setString(std::format("Bought spell {} for {}G.", select->getName(), select->getBuy()));
		}
		else if (!buy) {
			Game_Manager::player.setGold(Game_Manager::player.getGold() + select->getSell());
			texts[3].setString(std::format("Sold spell {} for {}G.", select->getName(), select->getSell()));
			Game_Manager::delSelectedTool(SpellTool);
			Game_Manager::organizeTool(SpellTool);
		}
		Game_Manager::selectedSpell = SelectNone;
	}

	for (unsigned int i = 0; i < stocks.size(); i++) {
		Spell* spell = &stocks[i];
		// Selects a spell
		if (spell->contains(slotX, slotY)) {
			int sx = spell->getPos('x'), sy = spell->getPos('y');
			Game_Manager::selectedSpell = i;
			map_txts["inv_sp_detail"].setString(spell->getDesc());
			map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
			return;
		}
	}
}

void Shop_Screen::shopSpecial(bool buy) {
	if (!buy)
		return;

	std::vector<Special>& stocks = Game_Manager::stockSpecial;
	Special* select = Game_Manager::selectedSpecial != SelectNone ? &stocks[Game_Manager::selectedSpecial] : NULL;

	if (select && mouseInButton(ConfirmButton)) {
		if (buy && Game_Manager::player.useGold(select->getBuy())) {
			select->use();
			texts[3].setString(std::format("Bought {} for {}G.", select->getName(), select->getBuy()));
		}
		Game_Manager::selectedSpecial = SelectNone;
	}

	for (unsigned int i = 0; i < stocks.size(); i++) {
		Special special = stocks[i];
		// Selects a special
		if (special.contains(slotX, slotY)) {
			int sx = special.getPos('x'), sy = special.getPos('y');
				Game_Manager::selectedSpecial = i;
				map_txts["inv_sp_detail"].setString(special.getDesc());
				map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
				return;
		}
	}
}

void Shop_Screen::resetSelect() {
	Game_Manager::selectedSpell = SelectNone;
	Game_Manager::selectedInv = SelectNone;
	Game_Manager::selectedSpecial = SelectNone;
}

void Shop_Screen::updateDraw() {
	texts[1].setString(std::format("{}/{} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));
	texts[2].setString(std::format("{}/{} items",
		Game_Manager::items.size(), Game_Manager::player.getMaxItems()));

	map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.getGold()));

	curSlots = buy ? Spell::spells.size() : Game_Manager::spells.size();
	switch (stock) {
	case ItemTool:
		curSlots = buy ? Item::items.size() : Game_Manager::items.size();
		break;
	case SpecialTool:
		curSlots = Special::specials.size();
		break;
	};

	update = false;
}