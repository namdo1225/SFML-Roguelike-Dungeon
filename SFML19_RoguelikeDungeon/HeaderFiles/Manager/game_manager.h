/**
*
* File: game_manager.h
* Description: Contain the declaration of the Game_Manager class, which represents a class that manages the gameplay data.
*
*/

#include "include.h"
#include <iostream>
#include "Manager/log_manager.h"
#include "Manager/font_manager.h"
#include "Tool/special.h"

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

class Game_Manager : public Log_Manager {
private:
	/**
	* Constructor for Setting_Manager.
	*/
	Game_Manager();

	/**
	* Add enemies to the floor.
	*/
	static void ene_add();

	/**
	* Handles all enemies making an action.
	*/
	static void ene_action();

	/**
	* Handles an enemy attacking.
	*
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	static void ene_atk(unsigned int v);

	/**
	* Handles an enemy moving closer to the player.
	*
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	static void ene_mov_close(unsigned int v);

	/**
	* A helper function for enemy moving closer.
	*
	* Parameter:
	*	v: an unsigned int for enemy index.
	*	offset_x: an int for the x offset.
	* 	offset_y: an int for the y offset.
	*/
	static bool ene_mov_close_2(unsigned int v, int offset_x, int offset_y);

	/**
	* Handles an enemy making a random move.
	*
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	static void ene_rand_move(unsigned int v);

public:
	static std::shared_ptr<Item> placeholder_item;
	static std::shared_ptr<Spell> placeholder_spell;
	static std::shared_ptr<Special> placeholder_special;
	static unsigned int enemy_respawn;
	static int off_x, off_y;
	static bool floor_copied;

	// shortcut
	static int cur_it_shortcut;
	static int cur_sp_shortcut;

	static Player player;
	static Floor floor;
	static std::vector<Enemy> enemies;

	static std::vector<std::shared_ptr<Item>> items;
	static std::vector<std::shared_ptr<Spell>> spells;
	static std::shared_ptr<Item> pl_weapon;
	static std::shared_ptr<Item> pl_armor;

	static std::shared_ptr<Item> inv_draw_desc;
	static std::shared_ptr<Item> inv_select;

	static std::shared_ptr<Spell> spell_select;
	static std::shared_ptr<Spell> spell_desc;

	static std::shared_ptr<Special> special_select;
	static std::shared_ptr<Special> special_desc;

	// shop
	static std::vector<std::shared_ptr<Item>> item_stocks;
	static std::vector<std::shared_ptr<Spell>> spell_stocks;
	static std::vector<std::shared_ptr<Special>> special_stocks;

	static void setup();

	/**
	* Handles enemy's death.
	*/
	static void ene_dead();

	/**
	* Organize player's inventory.
	*/
	static void reorganize_inv();

	/**
	* Picks an item to be on the quick item slot/shortcut.
	*
	* Parameter:
	*	place: a char for how the next item will be picked. 's' to select && use item.
	*		'r' or 'l' to select the left/right item of the currently selected item in
	*		inventory.
	*/
	static void itm_select_shortcut(char place);

	/**
	* Picks a spell to be on the quick spell slot/shortcut.
	*
	* Parameter:
	*	place: a char for how the next spell will be picked. 's' to select && use spell.
	*		'r' or 'l' to select the left/right spell of the currently selected spell in
	*		inventory.
	*/
	static void sp_select_shortcut(char place);

	/**
	* Handles player attacking with a spell.
	*/
	static void pl_sp_atk(unsigned int en_i, std::array<int, 4> sp_inf);

	/**
	* Handles player moving or attacking.
	*
	* Parameter:
	*	input: char. 'u', 'l', 'r', && 'd' for up, left, right, && down.
	*	mode: an unsigned int for the mode. 0 = move. 1 = attack.
	*/
	static void handle_player_action(char input, unsigned int mode);

	/**
	* Handles moving to the next floor.
	*/
	static void next_level(bool bypass = false);

	/**
	* Handles event for picking up item.
	*/
	static void handle_move_pick_itm();

	/**
	* Handles event for picking up gold.
	*/
	static void handle_move_pick_gld();

	/**
	* Handles event for touching an interactible.
	*/
	static void handle_move_pick_interact();

	/**
	* Handles player attacking enemy.
	*/
	static void pl_atk();

	/**
	* Refreshes/updates player's exp, including leveling them up.
	*/
	static void refresh_exp();

	/**
	* Randomize player's position.
	*/
	static void pl_random_pos();

	/**
	* Center all overworld objects relative to the player.
	*/
	static void center_floor();

	/**
	* Add an item to the inventory.
	*
	* Parameter:
	*	item: An Item* for the item to be added.
	*/
	static void add_item(std::shared_ptr<Item> item);

	/**
	* Reset the game in preparation for a new playthrough.
	*/
	static void reset_game();

	/**
	* Add a spell to the spell list.
	*
	* Parameter:
	*	spell: A std::shared_ptr<Spell> object for the spell to be added.
	*/
	static void add_spell(std::shared_ptr<Spell> spell);

	/**
	* Equip a weapon.
	*
	* Parameter:
	*	weapon: An std::shared_ptr<Item> object for the weapon to be equipped.
	*/
	static void equip_weapon(std::shared_ptr<Item> weapon);

	/**
	* Equip an armor.
	*
	* Parameter:
	*	armor: An std::shared_ptr<Item> object for the armor to be equipped.
	*/
	static void equip_armor(std::shared_ptr<Item> armor);

	/**
	* Handles player dying and resetting the game. The title screen is loaded again.
	*/
	static bool game_over();

	/**
	* Check if any obstacles (enemies, walls, etc) are stopping the player from moving.
	*/
	static void pl_move_obstacle();

	/**
	* Save the status of the current playthrough into a file.
	*/
	static void save();

	/**
	* Read the content of a save file.
	* 
	* Parameter:
	*	file_name: a string for the name of the save file.	
	*/
	static bool read_save(const char* name);

	/**
	* Organize player's spell list.
	*/
	static void reorganize_spell();

	/*
	* Delete selected item.
	*/
	static void delete_selected_itm();

	/*
	* Delete selected spell.
	*/
	static void deleted_selected_sp();

	/*
	* Reorganize spells.
	*/
	static void sp_reorganize();

	/*
	* Use an item.
	*/
	static void item_use();

	/*
	* Use a spell.
	*/
	static bool spell_use();

	/*
	* Handles a turn in the game world.
	*/
	static void handle_turn();
};


#endif