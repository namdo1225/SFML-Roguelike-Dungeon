/**
*
* File: spell.cpp
* Description: Contains implementations of Spell class and its sub-classes.
*
*/

#include "Tool/spell.h"
#include "Manager/font_manager.h"
#include "Manager/game_manager.h"
#include <format>

Spell::Spell(const char abbre[3],
	unsigned int id, unsigned int buy, unsigned int sell, unsigned int use,
	unsigned int range, unsigned int type, unsigned int mp,
	const char* desc, const char* name): uses(use), mp(mp) {
	this->id = id;
	this->buy_gd = buy;
	this->sell_gd = sell;
	this->type = type;
	this->range = range;
	this->name = name;
	this->desc.setString(std::format("{}\n\n{}\n\nBUY: {}G\nSELL: {}G\n\n-{}MP", name, desc, buy_gd, sell_gd, mp));

	icon.setString(abbre);
	txt.setString(abbre);
	txt.setPosition(1045, 310);
}

bool Spell::use() {
	long pl_mp = Game_Manager::player.get_stat(Mp);
	if (pl_mp < mp)
		return false;
	Game_Manager::player.set_stat(Mp, pl_mp - mp);
	return true;
}

std::array<int, 4> Spell::atk() { 
	std::array<int, 4> return_arr = { quantity + Game_Manager::player.get_stat(Mgk) * percentage, range, type, mp };
	return return_arr;
}

unsigned int Spell::get_use() { return uses; }

std::shared_ptr<Spell> Spell::create_spell(unsigned int id) {
	switch (id) {
	case 1:
		return std::make_shared<Heal>();
	case 2:
		return std::make_shared<Damage_All>();
	case 3:
		return std::make_shared<Fire>();
	case 4:
		return std::make_shared<Strength_Up>();
	case 5:
		return std::make_shared<Defense_Up>();
	case 6:
		return std::make_shared<Water>();
	case 7:
		return std::make_shared<Lightning>();
	case 8:
		return std::make_shared<Earth>();
	case 9:
		return std::make_shared<Light>();
	case 10:
		return std::make_shared<Dark>();
	}
}

PH_Spell::PH_Spell() : Spell("PH", 0, 0, 0, 0, 0, 0, 0, "Placeholder Spell", "Placeholder Spell") {}

Heal::Heal() : Spell("HE", 1, 50, 20, 3, 0, 0, 3,
	"Heal the player.\n\nPLAYER HP+: 5 + (MGK * 0.5)", "Heal") {
	quantity = 5;
}

bool Heal::use() {
	unsigned int hp = Game_Manager::player.get_stat(Hp);
	unsigned int max_hp = Game_Manager::player.get_stat(Max_Hp);
	long pl_mp = Game_Manager::player.get_stat(Mp);
	unsigned int mgk = Game_Manager::player.get_stat(Mgk);

	if (hp == max_hp || pl_mp < mp)
		return false;

	unsigned int new_hp = hp + quantity + mgk * 0.5;
	Game_Manager::player.set_stat(Mp, pl_mp - mp);
	Game_Manager::player.set_stat(Hp, new_hp > max_hp ? max_hp : new_hp);
	return true;
}

Damage_All::Damage_All() : Spell("DA", 2, 100, 40, 1, 0, 0, 8,
	"Damage all enemies in the \nroom (ignore defense \nand resistance).\n\nENEMY HP-: 4 + (MGK * 0.10)", "Damage All") {
	quantity = 4;
}

bool Damage_All::use() {
	if (!Spell::use())
		return false;

	unsigned int mgk = Game_Manager::player.get_stat(Mgk);

	for (unsigned int i = 0; i < Game_Manager::enemies.size(); i++)
		Game_Manager::enemies[i].set_stat(0, Game_Manager::enemies[i].get_stat(0) - quantity + mgk * 0.10);
	return true;
}

Fire::Fire() : Spell("FI", 3, 10, 4, 4, 3, 3, 4,
	"Hit an enemy with fire 3 \nspaces away.\n\nENEMY HP-: 4 + (MGK * \n0.5) - ENEMY RES", "Fire") {
	quantity = 4;
}

Strength_Up::Strength_Up() : Spell("SU", 4, 200, 50, 3, 0, 0, 5,
	"Increases strength for 8 turns.\n\nPLAYER STR+: 3 + (MGK * 0.25)", "Strength Up") {
	quantity = 3;
}

bool Strength_Up::use()
{
	if (!Spell::use())
		return false;

	Game_Manager::player.set_effect(Str, quantity + Game_Manager::player.get_stat(Mgk) * 0.25, 8);
	return true;
}

Defense_Up::Defense_Up() : Spell("DU", 5, 200, 50, 3, 0, 0, 5,
	"Increases strength for 10 turns.\n\nPLAYER STR+: 3 + (MGK * 0.20)", "Defense Up") {
	quantity = 3;
}

bool Defense_Up::use()
{
	if (!Spell::use())
		return false;

	Game_Manager::player.set_effect(Def, quantity + Game_Manager::player.get_stat(Mgk) * 0.20, 8);
	return true;
}

Water::Water() : Spell("WA", 4, 15, 5, 4, 5, 3, 4,
	"Hit an enemy with water 5 \nspaces away.\n\nENEMY HP-: 3 + (MGK * \n0.5) - ENEMY RES", "Water") {
	quantity = 3;
}

Lightning::Lightning() : Spell("LI", 5, 30, 7, 4, 2, 3, 4,
	"Hit an enemy with lightning 2 \nspaces away.\n\nENEMY HP-: 7 + (MGK * \n0.5) - ENEMY RES", "Lightning") {
	quantity = 7;
}

Earth::Earth() : Spell("EA", 6, 40, 10, 4, 4, 3, 4,
	"Hit an enemy with earth 4 \nspaces away.\n\nENEMY HP-: 5 + (MGK * \n0.75) - ENEMY RES", "Earth") {
	quantity = 5;
	percentage = 0.75;
}

Light::Light() : Spell("LT", 7, 40, 10, 4, 7, 3, 4,
	"Hit an enemy with light 7 \nspaces away.\n\nENEMY HP-: 3 + (MGK * \n0.75) - ENEMY RES", "Light") {
	quantity = 3;
	percentage = 0.75;
}

Dark::Dark() : Spell("DA", 8, 50, 15, 4, 2, 3, 4,
	"Hit an enemy with dark 1 \nspace away.\n\nENEMY HP-: 2 + MGK -\nENEMY RES", "Dark") {
	quantity = 2;
	percentage = 1;
}