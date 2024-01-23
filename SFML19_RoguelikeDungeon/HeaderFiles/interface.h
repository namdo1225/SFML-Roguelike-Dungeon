/**
* 
* File: interface.h
* Description: This class controls the entire game essentially, creating the interface
* and connect all other classes together. This class uses the Singleton pattern.
*
*/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "..\SFML-2.5.1/include/SFML/Audio.hpp"
#include <array>
#include "include.h"

class Interface {
protected:
	/**
	* Constructor for Interface.
	*/
	Interface();

	/**
	* Deconstructor for Interface.
	*/
	~Interface();

	static Interface* singleton;

private:
	sf::Font font;
	sf::RenderWindow window;
	sf::View viewUI{ sf::FloatRect(0, 0, 1200, 800) };
	sf::View viewWorld { sf::FloatRect(0, 0, 1200, 800) };

	int off_x{ 0 }, off_y{ 0 };

	// title_ui
	std::vector<sf::Text> title_texts;
	std::vector<sf::RectangleShape> title_rectangle;

	// name_creation_ui
	std::string name_ph_1{ "Player" };
	sf::Text name_ph_2, new_game_prompt;
	sf::RectangleShape name_line;

	// exit_ui
	std::vector<sf::Text> exit_texts;
	std::vector<sf::RectangleShape> exit_rectangles;

	// main_ui: (x = 100, y = 10, x_2 = 700, y_2 = 790) (sx = 600, sy = 780)
	// main border, main item border, main magic border, main history border, main level border
	std::vector<sf::RectangleShape> main_border_vector;

	// main player name, main player stat, main item text, main magic text, main history text
	std::vector<sf::Text> main_text, main_player_stats, main_player_stats_num;
	std::vector<sf::Text> main_level_text;

	sf::Text main_extra_menu_button;
	sf::RectangleShape main_extra_menu_rect;

	sf::Text main_current_floor_txt, main_gold_txt;

	sf::RectangleShape main_stat_popup_rect;
	sf::Text main_stat_popup_txt;

	std::vector<sf::Text> main_log;

	unsigned int enemy_respawn{ 5 };

	Item* select_itm{ new Place_Holder() };
	Spell* select_sp{ new PH_Spell() };
	int cur_it_shortcut{ 0 };
	int cur_sp_shortcut{ 0 };

	// stat_selection_ui:
	sf::Text stat_select_guide, stat_select_continue_txt;
	std::vector<sf::Text> stat_select_arrows, stats_text_draw, stats_draw, stats_left_draw;
	sf::RectangleShape stat_select_continue_rect;

	// level_up_ui (it uses some stat_selection variables):
	std::vector<sf::Text> level_up_arrows;
	sf::RectangleShape level_up_rect;
	sf::Text level_up_exit_txt;
	sf::RectangleShape level_up_exit_rect;
	std::array<unsigned int, 6> level_up_backup_stats;
	unsigned int level_up_backup_left;

	// extra_menu_ui:
	std::vector<sf::Text> extra_menu_txt;
	std::vector<sf::RectangleShape> extra_menu_rect;
	sf::RectangleShape extra_menu_black_bg;
	sf::Text extra_menu_help_txt;
	sf::RectangleShape extra_menu_help_rect;

	// inventory_menu_ui (it uses the exit button created in level up ui):
	sf::Text inv_title;

	// index 0 = weapon slot, 1 = chestplate slot, 2 = description box, 3 = use item button
	std::vector<sf::Text> inv_equipment_slot_txt;
	std::vector<sf::RectangleShape> inv_equipment_slot_rect;

	std::vector<sf::RectangleShape> inv_inventory_rect;

	std::vector<Item*> items;
	Item* pl_weapon{ new Place_Holder() };
	Item* pl_armor{ new Place_Holder() };

	Item* inv_draw_desc{ new Place_Holder() };
	Item* inv_select{ new Place_Holder() };
	sf::RectangleShape inv_select_rect;

	// spell_menu_ui (it uses the description box created in inventory ui && exit button in level up ui)
	sf::Text spell_title;
	std::vector<sf::RectangleShape> spell_slot_rect;
	std::vector<Spell*> spells;

	Spell* spell_desc{ new PH_Spell() };
	Spell* spell_select{ new PH_Spell() };

	bool sp_atk_screen{ false };

	// shop_menu_ui (description box from inv ui, exit button from lvl up ui)

	// 0: item, 1: spell, 2: buy, 3: sell
	std::array<sf::Text, 4> sh_type{ sf::Text(), sf::Text(), sf::Text(), sf::Text() };
	
	std::array<sf::Text, 2> sh_buy_sell_txt{ sf::Text(), sf::Text() };
	std::array<sf::RectangleShape, 30> sh_slots;
	std::array<Spell*, 3> sh_buy_sp{ new Heal(font), new Damage_All(font), new Fire(font) };
	std::array<Item*, 6> sh_buy_it{ new Health_Potion(font), new Mana_Potion(font), new Iron_Sword(font), new Iron_Armor(font),
	new Wooden_Staff(font), new Magic_Armor(font) };
	
	// load_menu_ui (exit button from lvl up ui)
	sf::Text ld_direction_txt, ld_savename_txt;
	std::string ld_savename_str;

	std::array<sf::Text, 4> shortcut_arrows{ sf::Text(), sf::Text(), sf::Text(), sf::Text() };

	Player player;
	Floor floor;
	std::vector<Enemy> enemies;

	bool title{ true }, exit_menu{ false }, name_screen{ false },
		stat_screen{ true }, lvl_screen{ false }, main_screen{ false },
		ext_screen{ false }, inv_screen{ false }, sp_screen{ false },
		map_menu{ false }, scan_popup{ false }, help_ui{ false }, level_up_reset{ false }, floor_copied{ false };

	sf::RectangleShape background;

	// sounds
	Audio_Manager aud;

	/**
	* Prevent instance from being copied.
	*/
	Interface(Interface& dup);

	/**
	* Prevent the use of operator= with this class instance.
	*/
	void operator=(const Interface&);

public:
	static Interface& get();

	// handle actual events

	/**
	* Handles moving to the next floor.
	*/
	void next_level();

	/**
	* Reset the game in preparation for a new playthrough.
	*/
	void reset_game();

	/**
	* Center all overworld objects relative to the player.
	*/
	void center_floor();

	/**
	* Check if any obstacles (enemies, walls, etc) are stopping the player from moving.
	*/
	void pl_move_obstacle();

	/**
	* The main method which contains the main loop for the game.
	*/
	void window_loop();

	/**
	* Draw the UI of the game.
	*/
	void draw_interface();

	/**
	* Has a loop to handle the events of the game.
	* 
	* Parameter:
	*	event: an sf::Event object that holds event data detected by the window.
	*/
	void handle_event(sf::Event& event);

	// handle save events
	/**
	* Save the status of the current playthrough into a file.
	*/
	void save();

	/**
	* Load the status of a saved playthrough from a file.
	*/
	void load();

	/**
	* Read the content of a save file.
	* 
	* Parameter:
	*	file_name: a string for the name of the save file.	
	*/
	void read_save(std::string file_name);

	// handle player events
	/**
	* Randomize player's position.
	*/
	void pl_random_pos();

	/**
	* Handles player attacking enemy.
	*/
	void pl_atk();

	/**
	* Refreshes/updates player's exp, including leveling them up.
	*/
	void refresh_exp();

	/**
	* Handles player attacking with a spell.
	*/
	void pl_sp_atk(Spell* spell);

	/**
	* Handles player dying && resetting the game. The title screen is loaded again.
	*/
	void pl_game_over();

	// handle enemy events
	/**
	* Add enemies to the floor.
	*/
	void ene_add();

	/**
	* Handles an enemy attacking.
	* 
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	void ene_atk(unsigned int v);

	/**
	* Handles an enemy moving closer to the player.
	* 
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	void ene_mov_close(unsigned int v);

	/**
	* A helper function for enemy moving closer.
	* 
	* Parameter:
	*	v: an unsigned int for enemy index.
	*	offset_x: an int for the x offset.
	* 	offset_y: an int for the y offset.
	*/
	bool ene_mov_close_2(unsigned int v, int offset_x, int offset_y);

	/**
	* Handles an enemy making a random move.
	* 
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	void ene_rand_move(unsigned int v);

	/**
	* Handles all enemies making an action.
	*/
	void ene_overall();

	/**
	* Checks && handles enemy dying.
	*/
	void ene_dead();

	/**
	* Handles an enemy making an action.
	*
	* Parameter:
	*	v: an unsigned int for enemy index.
	*/
	void ene_action(unsigned int v);

	// handle item events
	/**
	* Organize player's inventory.
	*/
	void itm_reoganize_inv();

	/**
	* Equip a weapon.
	*
	* Parameter:
	*	weapon: An Item* for the weapon to be equipped.
	*/
	void itm_equip_weapon(Item* weapon);

	/**
	* Add an item to the inventory.
	*
	* Parameter:
	*	item: An Item* for the item to be added.
	*/
	void itm_add_item(Item* item);

	/**
	* Equip an armor.
	*
	* Parameter:
	*	armor: An Item* for the armor to be equipped.
	*/
	void itm_equip_armor(Item* armor);

	/**
	* Picks an item to be on the quick item slot/shortcut.
	* 
	* Parameter:
	*	place: a char for how the next item will be picked. 's' to select && use item.
	*		'r' or 'l' to select the left/right item of the currently selected item in 
	*		inventory.
	*/
	void itm_select_shortcut(char place);

	// hand spell events
	/**
	* Add a spell to the spell list.
	*
	* Parameter:
	*	spell: An Spell* for the spell to be added.
	*/
	void sp_add_spell(Spell* spell);

	/**
	* Organize player's spell list.
	*/
	void sp_reorganize();

	/**
	* Picks a spell to be on the quick spell slot/shortcut.
	*
	* Parameter:
	*	place: a char for how the next spell will be picked. 's' to select && use spell.
	*		'r' or 'l' to select the left/right spell of the currently selected spell in
	*		inventory.
	*/
	void sp_select_shortcut(char place);

	// handle text log event
	/**
	* Adds a string to the text log.
	*
	* Parameter:
	*	string: an std::string to be added to the text log.
	*/
	void log_add(std::string string);

	// handle drawing objects setup
	/**
	* Set up objects needed to draw the title screen on the game window.
	*/
	void create_title_ui();

	/**
	* Set up objects needed to draw the name selection screen on the game window.
	*/
	void create_name_ui();

	/**
	* Set up objects needed to draw the exit screen on the game window.
	*/
	void create_exit_ui();

	/**
	* Set up objects needed to draw the stat selection screen on the game window.
	*/
	void create_stat_ui();

	/**
	* Set up objects needed to draw the main game screen on the game window.
	*/
	void create_main_ui();

	/**
	* Set up objects needed to draw the level up screen on the game window.
	*/
	void create_lvl_ui();

	/**
	* Set up objects needed to draw the extra menu on the game window.
	*/
	void create_ext_ui();

	/**
	* Set up objects needed to draw the inventory on the game window.
	*/
	void create_inv_ui();

	/**
	* Set up objects needed to draw the spell list menu on the game window.
	*/
	void create_sp_ui();

	/**
	* Set up objects needed to draw the shop menu on the game window.
	*/
	void create_shop_ui();

	/**
	* Set up objects needed to draw the load screen on the game window.
	*/
	void create_ld_ui();

	// handle draw to window
	/**
	* Draw the exit screen.
	*/
	void draw_exit_screen();

	/**
	* Draw the title screen.
	*/
	void draw_title_screen();

	/**
	* Draw the name creation screen.
	*/
	void draw_name_screen();

	/**
	* Draw the main screen.
	*/
	void draw_main_ui();

	/**
	* Draw the stat selection screen.
	*/
	void draw_stat_selection();

	/**
	* Draw the level up screen.
	*/
	void draw_lvl_ui();

	/**
	* Draw the extra menu screen.
	*/
	void draw_ext_ui();

	/**
	* Draw the inventory menu screen.
	*/
	void draw_inv_ui();

	/**
	* Draw the spell list screen.
	*/
	void draw_sp_ui();

	/**
	* Draw the map screen.
	*/
	void draw_map_ui();

	// handle button prompt
	/**
	* Handles event (keyboard) for typing in player's name.
	* 
	* Parameter:
	*	event: an sf::Event object with information on any events detected by the window.
	*/
	void name_creation(sf::Event& event);

	/**
	* Handles event (clicking) for the exit screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_exit_prompt(int x, int y);

	/**
	* Handles event (clicking) for the stat selection screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_stat_pick_prompt(int x, int y);

	/**
	* Handles event (clicking) for the level up screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_lvl_prompt(int x, int y);

	/**
	* Handles event (clicking) for the title screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_title_prompt(int x, int y);

	/**
	* Handles event (clicking) for the main screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_main_prompt(int x, int y);

	/**
	* Handles event (clicking) for the extra menu screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_ext_prompt(int x, int y);

	/**
	* Handles event (clicking) for the inventory screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_inv_prompt(int x, int y);

	/**
	* Handles event (clicking) for the spell list screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_spell_prompt(int x, int y);

	/**
	* Handles player moving or attacking.
	*
	* Parameter:
	*	input: char. 'u', 'l', 'r', && 'd' for up, left, right, && down.
	*	mode: an unsigned int for the mode. 0 = move. 1 = attack.
	*/
	void handle_player_action(char input, unsigned int mode);

	/**
	* Handles event for picking up item.
	*/
	void handle_move_pick_itm();

	/**
	* Handles event for picking up gold.
	*/
	void handle_move_pick_gld();

	/**
	* Handles event for touching an interactible.
	*/
	void handle_move_pick_interact();

	/**
	* Handles event (clicking) for the map screen.
	*
	* Parameter:
	*	x: an int for the mouse's x position.
	* 	y: an int for the mouse's y position.
	*/
	void handle_map_prompt(int x, int y);

	/**
	* Handles player making a spell attack
	*
	* Parameter:
	*	sp_inf: an std::array<int, 4> with information spell.
	*/
	void handle_sp_atk(std::array<int, 4> sp_inf);

	/**
	* Handles event (clicking) for the main shop screen.
	*/
	void handle_shop();

	/**
	* Handles event (clicking) for choosing buy or sell screen.
	* 
	* Parameter:
	*	buy_or_sell: a bool. true if buy. false if sell.
	*/
	void handle_sh_buy_and_sell(bool buy_or_sell);

	/**
	* Handles event (clicking) for the buy item screen.
	*/
	void handle_sh_wp_buy();

	/**
	* Handles event (clicking) for the sell item screen.
	*/
	void handle_sh_wp_sell();

	/**
	* Handles event (clicking) for the buy spell screen.
	*/
	void handle_sh_sp_buy();

	/**
	* Handles event (clicking) for the sell spell screen.
	*/
	void handle_sh_sp_sell();
};

#endif