#include "Manager/game_state_manager.h"
#include "Manager/log_manager.h"
#include "sf_manager.h"
#include <SFML/Window/Keyboard.hpp>
#include <Tool/item.h>
#include <Tool/tool.h>
#include <vector>

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

/**
* A class that manages the gameplay data.
*/
class Game_Manager : public Game_State_Manager, public Log_Manager, public SF_Manager {
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
	static void setup();

	/**
	* Handles enemy's death.
	*/
	static void delEnemy();

	/**
	* Organize player's tool.
	* 
	* Parameter:
	*	type: the type of tool to add.
	*/
	static void organizeTool(ToolEnum type);

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
	*/
	static void atkWithSpell(unsigned int enI);

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
	*	type: the type of tool to add.
	*/
	static void addTool(unsigned int id, ToolEnum type);

	/**
	* Reset the game in preparation for a new playthrough.
	* 
	* Parameter:
	*	cheat: true if player wants to cheat the game.
	*/
	static void resetGame(bool cheat = false);

	/**
	* Equip a weapon or armor.
	*
	* Parameter:
	*	position: The position of the weapon/armor in items.
	* 
	*/
	static void equipItem(unsigned int position, ItemType type);

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

	/*
	* Delete selected tool.
	* 
	* Parameter:
	*	type: The tool to delete
	*/
	static void delSelectedTool(ToolEnum type);

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

	/**
	* Get selected item.
	* 
	* Parameter:
	*	stocks: The stock of items to pick from.
	* 
	* Return:
	*	The selected item or null pointer.
	*/
	static Item* getSelectedItem(std::vector<Item>& stocks = items);
};


#endif