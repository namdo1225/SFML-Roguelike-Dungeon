/**
*
* File: item.cpp
* Description: Contains implementations of Item class and its sub-classes.
*
*/

#include "Manager/game_manager.h"
#include "Tool/item.h"
#include <functional>
#include <map>
#include <stat.h>
#include <Tool/tool.h>
#include <utility>

std::map<unsigned int, Item> Item::items;

Item::Item(unsigned int id, ItemType type, Stat stat, int quantity,
	unsigned int range, unsigned int buy, unsigned int sell, const char abbre[3],
	const char* desc, const char* name, std::function<void()> utilityUse):
	Tool(name, desc, abbre, id, buy, sell, quantity, range), type(type), stat(stat), utilityUse(utilityUse)
{}

Item::Item() : Tool("", "", "", 0, 0, 0, 0, 0) {}

Item::Item(unsigned int id) : Item(id, items[id].type, items[id].stat, items[id].quantity,
	items[id].range, items[id].buy, items[id].sell, items[id].abbrev, items[id].desc.c_str(), items[id].name, items[id].utilityUse) {}

bool Item::setup()
{
	unsigned int id = 0;
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Hp, 5, 0, 15, 3,
		"HI", "A potion that restore 5 HP.", "Health Potion")));
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Mp, 5, 0, 15, 3,
		"MI", "A potion that restore 5 MP.", "Mana Potion")));
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Hp, 10, 0, 15, 3,
		"RP", "A potion that restore 10 HP.", "Rejuvenate Potion")));
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Mp, 10, 0, 15, 3,
		"MP", "A potion that restore 10 MP.", "Magical Potion")));

	items.insert(std::make_pair(id++, Item(id, Weapon, Mgk, 2, 2, 50, 20,
		"WS", "A common staff allowing the use\nof magical attacks.\n\n2 RANGE",
		"Wooden Staff")));
	items.insert(std::make_pair(id++, Item(id, Weapon, Str, 3, 1, 40, 10,
		"IS", "A common sword.\n\n1 RANGE",
		"Iron Sword")));
	items.insert(std::make_pair(id++, Item(id, Weapon, Mgk, 4, 3, 100, 25,
		"MS", "A staff used by common mages.\n\n3 RANGE",
		"Mage Staff")));
	items.insert(std::make_pair(id++, Item(id, Weapon, Str, 6, 1, 80, 20,
		"SS", "A refined sword for warriors.\n\n1 RANGE",
		"Steel Sword")));

	items.insert(std::make_pair(id++, Item(id, Armor, Def, 3, 0, 40, 10,
		"IA", "A common armor.",
		"Iron Armor")));
	items.insert(std::make_pair(id++, Item(id, Armor, Res, 2, 0, 50, 20,
		"MA", "A common, magical armor.",
		"Magic Armor")));
	items.insert(std::make_pair(id++, Item(id, Armor, Def, 6, 0, 80, 20,
		"SA", "A basic but stable armor.",
		"Steel Armor")));

	return true;
}

void Item::use() {
	switch (type) {
	case StatConsumable:
		if (stat == Hp || stat == Mp) {
			unsigned int max = Game_Manager::player.get_stat(stat == Hp ? Max_Hp : Max_Mp);
			unsigned int cur = Game_Manager::player.get_stat(stat);
			unsigned int diff = max - cur;
			Game_Manager::player.set_stat(stat, diff > quantity ? cur + quantity : max);
		}

		return;
	case UtilityConsumable:
		if (utilityUse != NULL)
			utilityUse();
		return;
	}
}