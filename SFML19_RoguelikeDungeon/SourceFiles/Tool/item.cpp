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
#include <string>
#include <Tool/tool.h>
#include <utility>
#include <Manager/database_manager.h>

std::map<unsigned int, Item> Item::items;

Item::Item(unsigned int id, ItemType type, Stat stat, int quantity,
	unsigned int range, unsigned int buy, unsigned int sell, std::string abbre,
	std::string passedDesc, std::string name, std::function<void()> utilityUse):
	Tool(name, "", abbre, id, buy, sell, quantity, passedDesc, range), type(type), stat(stat), utilityUse(utilityUse) {
}

Item::Item() : Tool("", "", "", 0, 0, 0, 0, "", 0) {}

Item::Item(unsigned int id) : Item(id, items[id].type, items[id].stat, items[id].quantity,
	items[id].range, items[id].buy, items[id].sell, items[id].abbrev, items[id].originalDesc, items[id].name, items[id].utilityUse) {}

bool Item::setup()
{
	items.clear();
	Database_Manager::executeSelect("SELECT * FROM items;", [](void* data, int argc, char** argv, char** azColName) -> int {
		unsigned int i = std::stoi(argv[0]);

		items.insert(std::make_pair(i, Item(i, (ItemType)std::stoi(argv[8]), (Stat)std::stoi(argv[9]),
			std::stoi(argv[7]), std::stoi(argv[6]), std::stoi(argv[4]), std::stoi(argv[5]),
			argv[3], argv[2], argv[1])
		));

		return 0;
		});


	unsigned int id = 0;
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Hp, 5, 0, 15, 3,
		"HI", "A potion that restore 5 HP.", "Health Potion")));
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Mp, 5, 0, 15, 3,
		"MI", "A potion that restore 5 MP.", "Mana Potion")));
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Hp, 10, 0, 15, 3,
		"RP", "A potion that restore 10 HP.", "Rejuvenate Potion")));
	items.insert(std::make_pair(id++, Item(id, StatConsumable, Mp, 10, 0, 15, 3,
		"MP", "A potion that restore 10 MP.", "Magical Potion")));

	return true;
}

void Item::use() {
	switch (type) {
	case StatConsumable:
		if (stat == Hp || stat == Mp) {
			unsigned int max = Game_Manager::player.getStat(stat == Hp ? Max_Hp : Max_Mp);
			unsigned int cur = Game_Manager::player.getStat(stat);
			unsigned int diff = max - cur;
			Game_Manager::player.setStat(stat, diff > quantity ? cur + quantity : max);
		}

		return;
	case UtilityConsumable:
		if (utilityUse != NULL)
			utilityUse();
		return;
	}
}

unsigned int Item::getType() {
	return type;
}

Stat Item::getStat() {
	return stat;
}
