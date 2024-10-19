#include "Tool/special.h"
#include <Floor/enemy.h>
#include <Floor/floor.h>
#include <player.h>
#include <Tool/item.h>
#include <Tool/spell.h>
#include <vector>

#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

/**
* Enum for the items array, to help select items properly.
*/
enum SelectedItem {
	SelectWeapon = -3,
	SelectArmor,
	SelectNone
};

/**
* A class that manages the gameplay data.
*/
class Game_State_Manager {
public:
	static unsigned int enemyRespawns;
	static int offX, offY;
	static bool floorCopied;

	// shortcut
	static int itemQuickIndex;
	static int spellQuickIndex;

	static Player player;
	static Floor floor;
	static std::vector<Enemy> enemies;

	static std::vector<Item> items;
	static std::vector<Spell> spells;

	static Item plWeapon;
	static Item plArmor;

	static int selectedInv;
	static int selectedSpell;
	static int selectedSpecial;

	// shop
	static std::vector<Item> stockItem;
	static std::vector<Spell> stockSpell;
	static std::vector<Special> stockSpecial;
};


#endif