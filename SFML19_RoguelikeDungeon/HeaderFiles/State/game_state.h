#include "Tool/special.h"
#include <Floor/enemy.h>
#include <Tool/item.h>
#include <Tool/spell.h>
#include <vector>
#include "State/floor_state.h"
#include "State/player_state.h"

#ifndef GAME_STATE_H
#define GAME_STATE_H

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
class Game_State : public Floor_State, public Player_State {
public:
	static unsigned int enemyRespawns;
	static int offX, offY;
	static bool floorCopied;

	// shortcut
	static int itemQuickIndex;
	static int spellQuickIndex;

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