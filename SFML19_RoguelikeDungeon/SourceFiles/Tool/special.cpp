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
#include <string>
#include <Tool/tool.h>
#include <utility>
#include <format>

std::map<unsigned int, Special> Special::specials;

void Special::use() {
	useSpecial();
}

Special::Special(unsigned int id, unsigned int buy, unsigned int sell, std::string abbre, std::string desc, std::string name, std::function<void()> useSpecial) :
	Tool(name, "", abbre, id, buy, sell, 0, desc, 0), useSpecial(useSpecial) {
	std::string origin = desc;
	originalDesc = origin;
}

bool Special::setup()
{
	unsigned int id = 0;
	specials.insert(std::make_pair(id++,
		Special(id, 100, 0, "SP", "Increase item limit by 1.\nMax storage is 32.", "Storage Perk", []() {
			if (Game_Manager::player.getMaxItems() < Game_Manager::MAX_INV_SPELL_SLOTS) {
				Game_Manager::player.setMaxItem(Game_Manager::player.getMaxItems() + 1);
				Game_Manager::log_add("Your item limit increased by 1.");
			}
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "LP", "Move up a floor. Stay in the shop\nuntil you close the window.", "Ladder Perk", []() {
			Game_Manager::goUpFloor(true);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "SB", "Increase STR by 5 for 20 turns.", "Strength Boost", []() {
			Game_Manager::player.setEffect(Str, 5, 20);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "MB", "Increase MGK by 10 for 10 turns.", "Magic Boost", []() {
			Game_Manager::player.setEffect(Mgk, 10, 10);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "DB", "Increase DEF by 3 for 30 turns.", "Defense Boost", []() {
			Game_Manager::player.setEffect(Def, 3, 30);
		})));
	specials.insert(std::make_pair(id++,
		Special(id, 500, 0, "RB", "Increase RES by 1 for 50 turns.", "Resistance Boost", []() {
			Game_Manager::player.setEffect(Mgk, 1, 50);
		})));

	return true;
}