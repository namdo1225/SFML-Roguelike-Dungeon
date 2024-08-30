/**
* 
* File: player.cpp
* Description: This file contains the implementation for the Player class.
*
*/

#include "env.h"
#include "Manager/texture_manager.h"
#include "player.h"
#include <array>
#include <climits>
#include <effect.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <stat.h>
#include <string>
#include <vector>

Player::Player() {
	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::White);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::player);
}

void Player::set_effect(Stat stat, int quantity, unsigned int longevity, unsigned int current) {
	if (current == 0)
		effects.push_back(Effect(stat, quantity, longevity));
	else {
		effects.push_back(Effect(stat, quantity, longevity, current));
		if (current != longevity)
			effects[effects.size() - 1].effect_applied = true;
	}
}

int Player::attack_pl(bool atk_type, int uncalculated_quantity) {
	int quantity{ 1 };
	Stat type = atk_type ? Def : Res;

	if (stat[type] < uncalculated_quantity && uncalculated_quantity > 0)
		quantity = uncalculated_quantity - stat[type];
	stat[Hp] -= quantity;

	return quantity;
}

void Player::use_mp(int quantity) { stat[Mp] -= quantity; }

long Player::get_stat(Stat stat) { return this->stat[stat]; }

int Player::get_pos(char z) { return (z == 'x') ? getPosition().x : getPosition().y; }

bool Player::use_gold(unsigned int quantity) {
	if (quantity > gold)
		return false;
	gold -= quantity;
	return true;
}

std::string Player::get_name() { return name; }

unsigned int Player::get_lvl() { return level; }

unsigned int Player::get_cur_exp() { return cur_exp; }

unsigned int Player::get_lvl_up() { return lvl_up; }

unsigned int Player::get_gold() { return gold; }

unsigned int Player::get_pts() { return points; }

unsigned int Player::get_max_itm() { return max_item; }

unsigned int Player::get_floor() { return floor; }

void Player::set_name(std::string name) { this->name = name; }

void Player::set_floor(unsigned int fl_num) { floor = (fl_num % INT_MAX) == 0 ? 1 : fl_num; }

void Player::set_level(unsigned int t_lvl) { level = t_lvl; }

void Player::set_gold(unsigned int t_gold) { gold = t_gold; }

void Player::set_point(unsigned int pts) { points = pts; }

void Player::set_cur_exp(unsigned int curr_exp) { cur_exp = curr_exp; }

void Player::set_lvl_up(unsigned int lvl_exp) { lvl_up = lvl_exp; }

void Player::set_max_item(unsigned int slots) { max_item = slots; }

void Player::set_stat(Stat t_stat, unsigned int num) { stat[t_stat] = num; }

bool Player::is_dead() { return (stat[Hp] <= 0); }

void Player::reset(bool cheat, bool replaceName) {
	if (MODIFY_START_STATS)
		stat = STARTING_STATS;
	else if (cheat)
		stat = { 999, 999, 999, 999, 999, 999, 999, 999 };
	else
		stat = { 10, 5, 0, 0, 0, 0, 10, 5 };

	if (replaceName) name = "Player";
	points = STARTING_STAT_PTS ? STARTING_STAT_PTS : 10;
	level = 1;
	floor = STARTING_FLOOR ? STARTING_FLOOR : 1;
	cur_exp = 0;
	gold = cheat ? 10000 : STARTING_GOLD;
	lvl_up = 10;
}

bool Player::is_stuck(unsigned int i) { return stuck[i]; }

void Player::set_pos(int x, int y) { setPosition(x, y); }

void Player::set_stuck(unsigned int i, bool j) { stuck[i] = j; }

void Player::copy_stat(std::array<long, 8>& stats) { stats = stat; }

void Player::use_effect() {
	for (int i = effects.size() - 1; i >= 0; i--) {
		Stat type = effects[i].stat_changed;
		int difference = effects[i].stat_difference;
		long cur_stat = get_stat(type);

		if (effects[i].change_turns < 1) {
			if (type > Max_Mp && type < Hp && effects[i].effect_applied)
				set_stat(type, cur_stat - difference);
			effects.erase(effects.begin() + i);
			continue;
		}

		if (type > Res) {
			long new_quantity = cur_stat + difference;
			long max = get_stat(type == Hp ? Max_Hp : Max_Mp);
			if (new_quantity <= 0)
				set_stat(type, 1);
			else if (max <= new_quantity)
				set_stat(type, max);
			else
				set_stat(type, new_quantity);
		}
		else if (type > Max_Mp && type < Hp && effects[i].change_turns == effects[i].original_turns
			&& cur_stat + difference > 0 && cur_stat + difference < INT_MAX) {
			set_stat(type, cur_stat + difference);
			effects[i].effect_applied = true;
		}
		effects[i].change_turns--;
	}
}

void Player::reset_effect() {
	effects.clear();
}

std::vector<Effect> Player::get_effects() {
	return effects;
}

sf::FloatRect Player::get_rect() {
	return getGlobalBounds();
}
