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

const unsigned int Spell::SPELLS;

Spell::Spell(const char abbre[3],
	unsigned int id, unsigned int buy, unsigned int sell, unsigned int use,
	unsigned int range, unsigned int type, unsigned int mp,
	const char* desc): uses(use), mp(mp) {
	this->id = id;
	this->buy_gd = buy;
	this->sell_gd = sell;
	this->type = type;
	this->range = range;
	this->desc.setString(std::format("{}\n\nBUY: {}G\nSELL: {}G\n\n-{}MP", desc, buy_gd, sell_gd, mp));

	icon.setString(abbre);
	txt.setString(abbre);
	txt.setPosition(1045, 310);
}

void Spell::use() {}

std::array<int, 4> Spell::atk() { return {0,0,0,0}; };

unsigned int Spell::get_use() { return uses; }

std::shared_ptr<Spell> Spell::create_spell(unsigned int id) {
	switch (id) {
	case 1:
		return std::make_shared<Heal>();
	case 2:
		return std::make_shared<Damage_All>();
	case 3:
		return std::make_shared<Fire>();
	}
}

PH_Spell::PH_Spell() : Spell("PH", 0, 0, 0, 0, 0, 0, 0, "Placeholder Spell") {}

Heal::Heal() : Spell("HE", 1, 50, 20, 3, 0, 0, 3,
	"Heal\n\nHeal the player.\n\nPLAYER HP+: 5 + (MGK * 0.5)") {}

void Heal::use() {
	unsigned int hp = Game_Manager::player.get_stat(Hp);
	unsigned int max_hp = Game_Manager::player.get_stat(Max_Hp);
	unsigned int pl_mp = Game_Manager::player.get_stat(Mp);
	unsigned int mgk = Game_Manager::player.get_stat(Mgk);

	if (hp == max_hp)
		return;

	unsigned int new_hp = hp + 5 + mgk * 0.5;
	Game_Manager::player.set_stat(Mp, pl_mp - mp);
	Game_Manager::player.set_stat(Hp, new_hp > max_hp ? max_hp : new_hp);
}

Damage_All::Damage_All() : Spell("DA", 2, 100, 40, 1, 0, 0, 8,
	"Damage All\n\nDamage all enemies in the \nroom (ignore defense \nand resistance).\n\nENEMY HP-: 4 + (MGK * 0.10)") {}

void Damage_All::use() {
	unsigned int pl_mp = Game_Manager::player.get_stat(Mp);
	if (pl_mp < mp)
		return;

	unsigned int mgk = Game_Manager::player.get_stat(Mgk);
	Game_Manager::player.set_stat(Mp, pl_mp - 8);

	for (unsigned int i = 0; i < Game_Manager::enemies.size(); i++)
		Game_Manager::enemies[i].set_stat(0, Game_Manager::enemies[i].get_stat(0) - 4 + mgk * 0.10);
}

Fire::Fire() : Spell("FI", 3, 10, 4, 4, 3, 3, 4,
	"Fire \n\Hit an enemy with fire 3 \nspaces away.\n\nENEMY HP-: 4HP + (MGK * \n0.5) - ENEMY RES") {
	quantity = 4;
}

std::array<int, 4> Fire::atk() {
	std::array<int, 4> return_arr = { quantity + Game_Manager::player.get_stat(Mgk) * 0.5, range, type, mp};
	return return_arr;
}