/**
*
* File: special.cpp
* Description: Contains the implementation of the Special class.
*
*/

#include "Manager/game_manager.h"
#include "Tool/special.h"
#include <functional>
#include <map>
#include <stat.h>
#include <Tool/tool.h>
#include <utility>

std::map<unsigned int, Special> Special::specials;

void Special::use() {
	useSpecial();
}

Special::Special(unsigned int id, unsigned int buy, unsigned int sell, const char abbre[3], const char* desc, const char* name, std::function<void()> useSpecial) :
	Tool(name, desc, abbre, id, buy, sell, 0, 0), useSpecial(useSpecial) {}

bool Special::setup()
{
	unsigned int id = 0;
	specials.insert(std::make_pair(id++,
		Special(id, 100, 0, "SP", "Increase item limit by 1.\nMax storage is 32.", "Storage Perk", []() {
			if (Game_Manager::player.get_max_itm() < Game_Manager::MAX_INV_SPELL_SLOTS) {
				Game_Manager::player.set_max_item(Game_Manager::player.get_max_itm() + 1);
				Game_Manager::log_add("Your item limit increased by 1.");
			}
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "LP", "Move up a floor. Stay in the shop until you close the window.", "Ladder Perk", []() {
			Game_Manager::next_level(true);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "SB", "Increase STR by 5 for 20 turns.", "Strength Boost", []() {
			Game_Manager::player.set_effect(Str, 5, 20);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "MB", "Increase MGK by 10 for 10 turns.", "Magic Boost", []() {
			Game_Manager::player.set_effect(Mgk, 10, 10);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "DB", "Increase DEF by 3 for 30 turns.", "Defense Boost", []() {
			Game_Manager::player.set_effect(Def, 3, 30);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "RB", "Increase RES by 1 for 50 turns.", "Resistance Boost", []() {
			Game_Manager::player.set_effect(Mgk, 1, 50);
		})));

	return true;
}