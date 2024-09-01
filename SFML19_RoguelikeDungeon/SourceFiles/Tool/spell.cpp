/**
*
* File: spell.cpp
* Description: Contains implementations of Spell class and its sub-classes.
*
*/

#include "Manager/game_manager.h"
#include "Tool/spell.h"
#include <array>
#include <format>
#include <functional>
#include <Manager/database_manager.h>
#include <map>
#include <stat.h>
#include <string>
#include <Tool/tool.h>
#include <utility>

std::map<unsigned int, Spell> Spell::spells;

Spell::Spell(std::string abbre,
	unsigned int id, unsigned int buy, unsigned int sell, SpellType type,
	unsigned int range, unsigned int mp, int quantity,
	std::string desc, std::string name, double percent, std::function<void(int, double)> functionalUse):
	Tool(name, std::format("{}\n\n{}\n\nBUY: {}G\nSELL: {}G\n\n-{}MP\n\nRANGE: {}\n\n{}", name, desc, buy, sell, mp, range, type == Offensive ?
		std::format("ENEMY HP-: {} + (MGK * {})\n- ENEMY RES", quantity, percent)
		: "").c_str(),
		abbre, id, buy, sell, quantity, range), type(type), mp(mp), functionalUse(functionalUse), percentage(percent) {
	originalDesc = desc;
}

Spell::Spell() : Tool("", "", "", 0, 0, 0, 0, 0) {}

Spell::Spell(unsigned int id) : Spell(spells[id].abbrev, id, spells[id].buy, spells[id].sell, spells[id].type, spells[id].range, spells[id].mp, spells[id].quantity,
	spells[id].desc.c_str(), spells[id].name, spells[id].percentage, spells[id].functionalUse) {
}

bool Spell::setup() {
	spells.clear();
	Database_Manager::executeSelect("SELECT * FROM spells;", [](void* data, int argc, char** argv, char** azColName) -> int {
		unsigned int i = std::stoi(argv[0]);

		spells.insert(std::make_pair(i, Spell(argv[3], i, std::stoi(argv[4]), std::stoi(argv[5]),
			Offensive, std::stoi(argv[6]), std::stoi(argv[8]), std::stoi(argv[7]),
			argv[2], argv[1], std::stod(argv[9])
		)));


		return 0;
	});



	unsigned int id = 0;
	spells.insert(std::make_pair(id++, Spell("HE", id, 50, 20, Functional, 0, 3, 5,
		"Heal the player.\n\nPLAYER HP+: 5 + (MGK * 0.5)", "Heal", 0.5f,
		[](int quantity, double percent) {
			const unsigned int hp = Game_Manager::player.get_stat(Hp);
			const unsigned int max_hp = Game_Manager::player.get_stat(Max_Hp);
			const unsigned int mgk = Game_Manager::player.get_stat(Mgk);

			unsigned int new_hp = hp + quantity + mgk * percent;
			Game_Manager::player.set_stat(Hp, new_hp > max_hp ? max_hp : new_hp);
	})));

	spells.insert(std::make_pair(id++, Spell("DA", id, 100, 40, Functional, 0, 8, 4,
		"Damage all enemies while\n ignoring DEF/RES.\n\nENEMY HP-: 4 + (MGK * 0.10)",
		"Damage All", 0.1f,
		[](int quantity, double percent) {
			const unsigned int mgk = Game_Manager::player.get_stat(Mgk);
			for (unsigned int i = 0; i < Game_Manager::enemies.size(); i++)
				Game_Manager::enemies[i].stat.hp -= quantity + mgk * percent;
	})));

	spells.insert(std::make_pair(id++, Spell("SU", id, 200, 50, Functional, 0, 5, 3,
		"Increases strength for 8 turns.\n\nPLAYER STR+: 3 + (MGK * 0.25)", "Strength Up", 0.25f,
		[](int quantity, double percent) {
			Game_Manager::player.set_effect(Str, quantity + Game_Manager::player.get_stat(Mgk) * percent, 8);
	})));
	spells.insert(std::make_pair(id++, Spell("DU", id, 200, 50, Functional, 0, 5, 3,
		"Increases strength for 10 turns.\n\nPLAYER STR+: 3 + (MGK * 0.20)", "Defense Up", 0.2f,
		[](int quantity, double percent) {
			Game_Manager::player.set_effect(Def, quantity + Game_Manager::player.get_stat(Mgk) * percent, 8);
	})));
	return true;
}

bool Spell::usePlayerMP() const {
	long pl_mp = Game_Manager::player.get_stat(Mp);
	if (pl_mp < mp)
		return false;
	Game_Manager::player.set_stat(Mp, pl_mp - mp);
	return true;
}

bool Spell::use() const {
	if (functionalUse != NULL && usePlayerMP()) {
		functionalUse(quantity, percentage);
		return true;
	}

	return false;
}

std::array<int, 3> Spell::atk() { 
	std::array<int, 3> return_arr = { quantity + Game_Manager::player.get_stat(Mgk) * percentage, range, mp };
	return return_arr;
}