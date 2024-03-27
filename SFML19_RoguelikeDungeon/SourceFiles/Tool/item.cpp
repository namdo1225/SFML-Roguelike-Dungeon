/**
*
* File: item.cpp
* Description: Contains implementations of Item class and its sub-classes.
*
*/

#include "Tool/item.h"
#include "Manager/font_manager.h"
#include "Manager/game_manager.h"
#include <format>

Item::Item(int id, int type, bool modify_act,
	bool hp_mp_other, Stat stat, int quantity,
	int range, int buy, int sell, const char abbre[3],
	const char* desc): modify_or_act(modify_act), hp_mp_other(hp_mp_other),
	stat(stat), quantity(quantity) {
	this->type = type;
	this->id = id;
	this->buy_gd = buy;
	this->sell_gd = sell;
	this->range = range;
	this->desc.setString(std::format("{}\n\n+{} {}\n\nBUY: {}G\nSELL: {}G", desc, quantity, STAT_STRINGS[stat], buy_gd, sell_gd));

	icon.setString(abbre);
	txt.setString(abbre);
	txt.setPosition(825, 310);
}

std::shared_ptr<Item> Item::create_itm(unsigned int id) {
	switch (id) {
	case 1:
		return std::make_shared<Health_Potion>();
	case 2:
		return std::make_shared<Mana_Potion>();
	case 3:
		return std::make_shared<Wooden_Staff>();
	case 4:
		return std::make_shared<Iron_Armor>();
	case 5:
		return std::make_shared<Magic_Armor>();
	case 6:
		return std::make_shared<Iron_Sword>();
	case 7:
		return std::make_shared<Steel_Sword>();
	case 8:
		return std::make_shared<Mage_Staff>();
	case 9:
		return std::make_shared<Steel_Armor>();
	}
}

Stat Item::get_stat() { return stat; }

int Item::get_quantity() { return quantity; }

bool Item::get_hp_mp_other() { return hp_mp_other; }

void Item::use() {}

Place_Holder::Place_Holder() : Item(0, 0, true, false, Max_Hp, 0, 0, 0, 0,
	"PH", "Place Holder") {
	type = 3;
}

Health_Potion::Health_Potion() : Item(1, 0, true, false, Max_Hp, 5, 0, 15, 3,
	"HI", "Health Potion\n\nA potion that restore a \nsmall amount of health.") {}

void Health_Potion::use() {
	unsigned int max_hp = Game_Manager::player.get_stat(Max_Hp);
	unsigned int hp = Game_Manager::player.get_stat(Hp);
	unsigned int difference = max_hp - hp;
	Game_Manager::player.set_stat(Hp, difference > 5 ? hp + 5 : max_hp);
}

Mana_Potion::Mana_Potion() : Item(2, 0, true, false, Max_Mp, 5, 0, 15, 3,
	"MI", "Mana Potion\n\nA potion that restore a \nsmall amount of mana.") {}

void Mana_Potion::use() {
	unsigned int max_mp = Game_Manager::player.get_stat(Max_Mp);
	unsigned int mp = Game_Manager::player.get_stat(Mp);
	unsigned int difference = max_mp - mp;
	Game_Manager::player.set_stat(Mp, difference > 5 ? mp + 5 : max_mp);
}

Wooden_Staff::Wooden_Staff() : Item(3, 1, true, true, Mgk, 2, 2, 50, 20,
	"WS", "Wooden Staff\n\nA common staff allowing the use\nof magical attacks.\n\n2 RANGE") {}

Iron_Armor::Iron_Armor() : Item(4, 2, true, true, Def, 3, 0, 40, 10,
	"IA", "Iron Armor\n\nA common armor.") {}

Magic_Armor::Magic_Armor() : Item(5, 2, true, true, Res, 2, 0, 50, 20,
	"MA", "Magic Armor\n\nA common magic armor.") {}

Iron_Sword::Iron_Sword() : Item(6, 1, true, true, Str, 3, 1, 40, 10,
	"IS", "Iron Sword\n\nA common sword.") {}

Steel_Sword::Steel_Sword() : Item(7, 1, true, true, Str, 6, 1, 80, 20,
	"SS", "Steel Sword\n\nA refined sword for warriors.") {}

Mage_Staff::Mage_Staff() : Item(8, 1, true, true, Mgk, 4, 3, 100, 25,
	"MS", "Mage Staff\n\nA staff used by common mages.\n\n3 RANGE") {}

Steel_Armor::Steel_Armor() : Item(9, 2, true, true, Def, 6, 0, 80, 20,
	"SA", "Steel Armor\n\nA basic but stable armor.") {}