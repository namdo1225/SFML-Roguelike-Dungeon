/**
*
* File: special.cpp
* Description: Contains the implementation of the Special class.
*
*/


#include "Tool/special.h"
#include "Manager/game_manager.h"
#include <format>

Special::Special(unsigned int id, unsigned int buy, unsigned int sell,
	const char abbre[3], const char* desc, const char* name) {
	this->id = id;
	this->buy_gd = buy;
	this->sell_gd = sell;
	this->name = name;
	this->desc.setString(std::format("{}\n\n{}\n\nBUY: {}G\n", name, desc, buy_gd));

	icon.setString(abbre);
	txt.setString(abbre);
}

std::shared_ptr<Special> Special::create_special(unsigned int id)
{
	switch (id) {
	case 1:
		return std::make_shared<Storage_Perk>();
	case 2:
		return std::make_shared<Floor_Perk>();
	case 3:
		return std::make_shared<Strength_Perk>();
	case 4:
		return std::make_shared<Magic_Perk>();
	case 5:
		return std::make_shared<Defense_Perk>();
	case 6:
		return std::make_shared<Resistance_Perk>();
	}
}

PH_Special::PH_Special() : Special(0, 0, 0, "PH", "Placeholder Special", "Placeholder Special") {}

void PH_Special::use() {}

Storage_Perk::Storage_Perk(): Special(1, 100, 0, "SP", "Increase item limit by 1.\nMax storage is 32.", "Storage Perk") {}

void Storage_Perk::use() {
	if (Game_Manager::player.get_max_itm() < Game_Manager::MAX_INV_SPELL_SLOTS) {
		Game_Manager::player.set_max_item(Game_Manager::player.get_max_itm() + 1);
		Game_Manager::log_add("Your item limit increased by 1.");
	}
}

Floor_Perk::Floor_Perk() : Special(2, 75, 0, "FP",
	"Buy this to move up a floor."
	"\nYou can stay in the shop"
	"\nuntil you close the window.",
	"Floor Perk"
) {}

void Floor_Perk::use() {
	Game_Manager::next_level(true);
}

Strength_Perk::Strength_Perk() : Special(3, 500, 0, "SI",
	"Increase STR by 5 for 20 turns.",
	"Strength Perk"
) {}

void Strength_Perk::use() {
	Game_Manager::player.set_effect(Str, 5, 20);
}

Magic_Perk::Magic_Perk() : Special(4, 500, 0, "MI",
	"Increase MGK by 10 for 10 turns.",
	"Magic Perk"
) {}

void Magic_Perk::use() {
	Game_Manager::player.set_effect(Mgk, 10, 10);
}

Defense_Perk::Defense_Perk() : Special(5, 500, 0, "DI",
	"Increase DEF by 3 for 30 turns.",
	"Defense Perk"
) {}

void Defense_Perk::use() {
	Game_Manager::player.set_effect(Def, 3, 30);
}

Resistance_Perk::Resistance_Perk() : Special(6, 500, 0, "RI",
	"Increase RES by 1 for 50 turns.",
	"Resistance Perk"
) {}

void Resistance_Perk::use() {
	Game_Manager::player.set_effect(Res, 1, 50);
}
