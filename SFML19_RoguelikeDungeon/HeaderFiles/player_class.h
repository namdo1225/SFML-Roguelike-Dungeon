/**
* 
* File: player_class.h
* Description: This class abstracts Player information. It contains information like player's hp && mp.
*
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include <array>
#include "effect.h"

class Player : sf::RectangleShape {
private:
	std::vector<Effect> effects;

	sf::Text name;

	// value: false = free, true = stuck, can't move
	// index: 0 = up, 1 = left, 2 = down, 3 = right
	std::array<bool, 4> stuck{ false, false, false, false };

	// CURRENT_HP, CURRENT_MP
	std::array<int, 2> cur_hp_mp = { 10, 5 };

	unsigned int points{ 10 }, level{ 1 }, cur_exp{ 0 }, lvl_up{ 10 }, floor{ 1 }, gold{ 0 }, max_item{ 10 };

	// MAX_HP, MAX_MP, STR, MAG, DEF, RES
	std::array<unsigned int, 6> stat = { 10, 5, 0, 0, 0, 0 };

	static sf::Texture pl_tex;

public:

	/**
	* Constructor for Player.
	*/
	Player();

	/**
	* Constructor for Player.
	* 
	* Parameter:
	*	font: an sf::Font object to hold font data for player's name.
	*/
	Player(sf::Font& font);

	/**
	* Getter for player's level.
	*
	* Return:
	*	an unsigned int for player's level.
	*/
	unsigned int get_lvl();

	/**
	* Getter for player's level.
	*
	* Return:
	*	an unsigned int for player's level.
	*/
	unsigned int get_cur_exp();

	/**
	* Getter for player's exp needed for level up.
	*
	* Return:
	*	an unsigned int for player's level-up exp.
	*/
	unsigned int get_lvl_up();

	/**
	* Getter for player's gold amount.
	*
	* Return:
	*	an unsigned int for player's gold amount.
	*/
	unsigned int get_gold();

	/**
	* Getter for player's stat points.
	*
	* Return:
	*	an unsigned int for player's stat points.
	*/
	unsigned int get_pts();

	/**
	* Getter for player's max item slots.
	*
	* Return:
	*	an unsigned int for player's max item slots.
	*/
	unsigned int get_max_itm();

	/**
	* Getter for player's current floor.
	*
	* Return:
	*	an unsigned int for player's floor.
	*/
	unsigned int get_floor();

	/**
	* Getter for player's stat.
	*
	* Parameter:
	*	t_stat: an unsigned int to index the stat type. 0-5 for the regular stat types. 6-7 for current hp/mp.
	* 
	* Return:
	*	an unsigned int for the value of stat type.
	*/
	unsigned int get_stat(unsigned int t_stat);

	/**
	* Getter for player's position.
	*
	* Parameter:
	*	z: a char to get either player's x ('x') or y ('y') position.
	* 
	* Return:
	*	an unsigned int for player's floor.
	*/
	int get_pos(char z);

	/**
	* Getter for player's name
	*
	* Return:
	*	an std::string for player's name.
	*/
	std::string get_name();

	/**
	* Check if player is stuck.
	*
	* Parameter:
	*	i: an unsigned int to check if the player is stuck in a certain direction.
	* 
	* Return:
	*	a bool. true if player is dead. false if player is alive.
	*/
	bool is_stuck(unsigned int i);

	/**
	* Check if player is dead (hp is 0 or smaller)
	*
	* Return:
	*	a bool. true if player is dead. false if player is alive.
	*/
	bool is_dead();

	/**
	* Method to set player's status effect.
	*
	* Parameter:
	*	stat: an unsigned int for which stat to affect.
	*	quantity: an int for how much to affect that stat.
	*	longevity: an unsigned int for how many turns the effect will apply for.
	*	current: an unsigned int for how many turns the effect has left (useful for loading from saves).
	*/
	void set_effect(unsigned stat, int quantity, unsigned int longevity, unsigned int current = 0);

	/**
	* Setter for player's name.
	*
	* Parameter:
	*	t_name: std::string for player's new name.
	*/
	void set_name(std::string t_name);

	/**
	* Setter for player's current floor.
	*
	* Parameter:
	*	fl_num: an unsigned int for player's new current floor.
	*/
	void set_floor(unsigned int fl_num);

	/**
	* Setter for player's current level.
	*
	* Parameter:
	*	t_lvl: an unsigned int for player's new level.
	*/
	void set_level(unsigned int t_lvl);

	/**
	* Setter for player's gold amount.
	*
	* Parameter:
	*	t_gold: an unsigned int for player's new gold amount.
	*/
	void set_gold(unsigned int t_gold);

	/**
	* Setter for player's stat points.
	*
	* Parameter:
	*	pts: an unsigned int for player's new stat points.
	*/
	void set_point(unsigned int pts);

	/**
	* Setter for player's current exp.
	*
	* Parameter:
	*	curr_exp: an unsigned int for player's new exp.
	*/
	void set_cur_exp(unsigned int curr_exp);

	/**
	* Setter for player's exp needed to level up.
	*
	* Parameter:
	*	lvl_exp: an unsigned int for the new level up exp.
	*/
	void set_lvl_up(unsigned int lvl_exp);

	/**
	* Setter for player's max item slots.
	*
	* Parameter:
	*	slots: an unsigned int for player's new max item slots.
	*/
	void set_max_item(unsigned int slots);

	/**
	* Setter for player's stat.
	*
	* Parameter:
	*	t_stat: an unsigned int to pick the stat type.
	*	num: an unsigned int for the new value for the stat type.
	*/
	void set_stat(unsigned int t_stat, unsigned int num);

	/**
	* Setter for player's position.
	* 
	* Default position is 400,400.
	* 
	* Parameter:
	*	x: an int for player's x position.
	* 	y: an int for player's y position.
	*/
	void set_pos(int x, int y);

	/**
	* Setter for player's stuck direction (they cannot move that way if stuck 
	* in that direction).
	*
	* Parameter:
	*	i: an unsigned int to for indexing of direction.
	*		0 = up, 1 = left, 2 = down, 3 = right
	*	j: a bool. true if stuck. false if not.
	*/
	void set_stuck(unsigned int i, bool j);

	/**
	* Use player's mp && calculate remaining mp.
	* 
	* Parameter:
	*	quantity: an int for the amount of mp that will be used.
	*/
	void use_mp(int quantity);

	/**
	* Attack player && calculate remaining hp.
	*
	* Parameter:
	*	atk_type: a bool for the attack type. true if physical. false if magical.
	*	uncalculated_quantity: an int for the damage amount.
	* 
	* Return:
	*	an int for the how much the player is actually damaged by.
	*/
	int attack_pl(bool atk_type, int uncalculated_quantity);
	
	/**
	* Spend player's gold.
	*
	* Parameter:
	*	quantity: an int for how much money to spend.
	*
	* Return:
	*	a bool. True if the player has that amount to spend. False if not.
	*/
	bool use_gold(int quantity);

	/**
	* Method to reset object's members to default value.
	*/
	void reset();

	/**
	* Draw player's name or sprite.
	*
	* Parameter:
	*	window: an sf::RenderWindow object to draw the player on.
	*	draw: a char to choose whether to draw name or sprite.
	*/
	void draw(sf::RenderWindow& window, char draw);

	/**
	* Copy player's stat for general usage.
	*
	* Parameter:
	*	stats: an std::array<unsigned int, 6> object to copy the stats to.
	*/
	void copy_stat(std::array<unsigned int, 6>& stats);

	/**
	* Handles effects on player.
	*/
	void use_effect();

	/**
	* Resets effects on player.
	*/
	void reset_effect();

	/**
	* Returns effects.
	* 
	* Return:
	*	A vector of Effects.
	*/
	std::vector<Effect> get_effects();

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
};

#endif