#include "Manager/log_manager.h"
#include "sf_manager.h"
#include "Tool/special.h"
#include <array>
#include <Floor/enemy.h>
#include <Floor/floor.h>
#include <player.h>
#include <SFML/Window/Keyboard.hpp>
#include <Tool/item.h>
#include <Tool/spell.h>
#include <vector>

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

/**
* A class that manages the gameplay data.
*/
class Game_Manager : public Log_Manager, public SF_Manager {
private:
	/**
	* Constructor for Setting_Manager.
	*/
	Game_Manager();

	/**
	* Add enemies to the floor.
	*/
	static void addEnemy();

	/**
	* Handles all enemies making an action.
	*/
	static void actEnemy();

	/**
	* Handles an enemy attacking.
	*
	* Parameter:
	*	v: Enemy's index.
	*/
	static void atkEnemy(unsigned int v);

	/**
	* Handles an enemy moving closer to the player.
	*
	* Parameter:
	*	v: Enemy's index.
	*/
	static void moveEnemyClose(unsigned int v);

	/**
	* A helper function for enemy moving closer.
	*
	* Parameter:
	*	v: Enemy's index.
	*	offsetX: x offset.
	* 	offsetY: y offset.
	* 
	* Return:
	*	Whether enemy can move closer.
	*/
	static bool moveEnemyCloseH(unsigned int v, int offsetX, int offsetY);

	/**
	* Handles an enemy making a random move.
	*
	* Parameter:
	*	v: Enemy's index.
	*/
	static void moveEnemyRand(unsigned int v);

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

	static Item* plWeapon;
	static Item* plArmor;

	static Item* selectedInv;
	static Spell* selectedSpell;
	static Special* selectedSpecial;

	// shop
	static std::vector<Item> stockItem;
	static std::vector<Spell> stockSpell;
	static std::vector<Special> stockSpecial;

	static void setup();

	/**
	* Handles enemy's death.
	*/
	static void delEnemy();

	/**
	* Organize player's inventory.
	*/
	static void organizeInv();

	/**
	* Picks an item to be on the quick item slot/shortcut.
	*
	* Parameter:
	*	leftOrRight: true for left.
	*/
	static void findItemShortcut(bool leftOrRight);

	/**
	* Picks a spell to be on the quick spell slot/shortcut.
	*
	* Parameter:
	*	leftOrRight: true for left.
	*/
	static void findSpellShortcut(bool leftOrRight);

	/**
	* Handles player attacking with a spell.
	* 
	* Parameter:
	*	enI: enemy's index.
	*	atkSpInfo: offensive spell's info on enemy.
	*/
	static void atkWithSpell(unsigned int enI, std::array<int, 3> atkSpInfo);

	/**
	* Handles player moving or attacking.
	*
	* Parameter:
	*	input: up, left, right, & down direction.
	*	mode: The mode. 0 = move. 1 = attack.
	*/
	static void handlePlayerAct(sf::Keyboard::Key input, unsigned int mode);

	/**
	* Handles moving to the next floor.
	* 
	* Parameter:
	*	bypass: true to bypass stair finding requirement to move up
	*		to the next floor.
	*/
	static void goUpFloor(bool bypass = false);

	/**
	* Handles event for picking up item.
	*/
	static void pickUpItem();

	/**
	* Handles event for picking up gold.
	*/
	static void pickUpGold();

	/**
	* Handles event for touching an interactible.
	*/
	static void stepOnInteractible();

	/**
	* Handles player attacking enemy.
	*/
	static void playerAttack();

	/**
	* Refreshes/updates player's exp, including leveling them up.
	*/
	static void updateEXP();

	/**
	* Randomize player's position.
	*/
	static void playerRandomPos();

	/**
	* Center all overworld objects relative to the player.
	*/
	static void centerFloor();

	/**
	* Add an item to the inventory.
	*
	* Parameter:
	*	id: id for item to be added.
	*/
	static void addItem(unsigned int id);

	/**
	* Reset the game in preparation for a new playthrough.
	* 
	* Parameter:
	*	cheat: true if player wants to cheat the game.
	*/
	static void resetGame(bool cheat = false);

	/**
	* Add a spell to the spell list.
	*
	* Parameter:
	*	id: id for spell to be added.
	*/
	static void addSpell(unsigned int id);

	/**
	* Equip a weapon.
	*
	* Parameter:
	*	weapon: The weapon to be equipped.
	*/
	static void equipWeapon(Item* weapon);

	/**
	* Equip an armor.
	*
	* Parameter:
	*	armor: The armor to be equipped.
	*/
	static void equipArmor(Item* armor);

	/**
	* Handles player dying and resetting the game. The title screen is loaded again.
	* 
	* Return:
	*	true if the game is over.
	*/
	static bool gameOver();

	/**
	* Check if any obstacles (enemies, walls, etc) are stopping the player from moving.
	*/
	static void checkPlayerPath();

	/**
	* Save the status of the current playthrough into a file.
	*/
	static void save();

	/**
	* Read the content of a save file.
	* 
	* Return:
	*	true if save is successfully read.
	*/
	static bool readSave();

	/**
	* Organize player's spell list.
	*/
	static void organizeSpell();

	/*
	* Delete selected item.
	*/
	static void delSelectedItem();

	/*
	* Delete selected spell.
	*/
	static void delSelectedSpell();

	/*
	* Use an item.
	*/
	static void useItem();

	/*
	* Use a spell.
	* 
	* Return:
	*	true if spell is used successfully.
	*/
	static bool useSpell();

	/*
	* Handles a turn in the game world.
	*/
	static void handleTurn();
};


#endif