/**
* 
* File: player.cpp
* Description: This file contains the implementation for the Player class.
*
*/

#include "player.h"
#include "texture_manager.h"

Player::Player() {
	name.setFillColor(sf::Color::Cyan);
	name.setStyle(sf::Text::Bold);
	name.setCharacterSize(12);
	name.setPosition(805, 15);
	name.setOutlineThickness(3.f);
	name.setOutlineColor(sf::Color::Black);

	setSize(sf::Vector2f(40, 40));
	setFillColor(sf::Color::White);
	setTextureRect(sf::IntRect(0, 0, 40, 40));
	setTexture(&Texture_Manager::player);
}

Player::Player(sf::Font& font) : Player() { name.setFont(font); }

void Player::set_effect(unsigned int stat, int quantity, unsigned int longevity, unsigned int current) {
	if (current == 0)
		effects.push_back(Effect(stat, quantity, longevity));
	else
		effects.push_back(Effect(stat, quantity, longevity, current));
}

int Player::attack_pl(bool atk_type, int uncalculated_quantity) {
	int quantity{ 1 };
	unsigned int type{ 4 };

	// check if attack type is magic
	if (!atk_type)
		type = 5;

	if (stat[type] < uncalculated_quantity)
		quantity = uncalculated_quantity - stat[type];

	cur_hp_mp[0] -= quantity;

	return quantity;
}

void Player::use_mp(int quantity) { cur_hp_mp[1] -= quantity; }

unsigned int Player::get_stat(unsigned int t_stat) { return (t_stat < 6) ? stat[t_stat] : cur_hp_mp[t_stat - 6]; }

int Player::get_pos(char z) { return (z == 'x') ? getPosition().x : getPosition().y; }

bool Player::use_gold(int quantity) {
	if (quantity > gold)
		return false;
	gold -= quantity;
	return true;
}

std::string Player::get_name() { return name.getString(); }

unsigned int Player::get_lvl() { return level; }

unsigned int Player::get_cur_exp() { return cur_exp; }

unsigned int Player::get_lvl_up() { return lvl_up; }

unsigned int Player::get_gold() { return gold; }

unsigned int Player::get_pts() { return points; }

unsigned int Player::get_max_itm() { return max_item; }

unsigned int Player::get_floor() { return floor; }

void Player::set_name(std::string t_name) { name.setString(t_name); }

void Player::set_floor(unsigned int fl_num) { floor = (fl_num % INT_MAX) == 0 ? 1 : fl_num; }

void Player::set_level(unsigned int t_lvl) { level = t_lvl; }

void Player::set_gold(unsigned int t_gold) { gold = t_gold; }

void Player::set_point(unsigned int pts) { points = pts; }

void Player::set_cur_exp(unsigned int curr_exp) { cur_exp = curr_exp; }

void Player::set_lvl_up(unsigned int lvl_exp) { lvl_up = lvl_exp; }

void Player::set_max_item(unsigned int slots) { max_item = slots; }

void Player::set_stat(unsigned int t_stat, unsigned int num) { (t_stat < 6) ? stat[t_stat] = num : cur_hp_mp[t_stat - 6] = num; }

bool Player::is_dead() { return (cur_hp_mp[0] <= 0); }

void Player::reset() {
	stat = { 10, 5, 0, 0, 0, 0 };
	name.setString("");
	points = 10;
	level = floor = 1;
	cur_exp = gold = 0;
	lvl_up = 10;
	cur_hp_mp = { 10, 5 };
}

bool Player::is_stuck(unsigned int i) { return stuck[i]; }

void Player::set_pos(int x, int y) { setPosition(x, y); }

void Player::set_stuck(unsigned int i, bool j) { stuck[i] = j; }

void Player::draw(sf::RenderWindow& window, char draw) { (draw == 's') ? window.draw(*this) : window.draw(name); }

void Player::copy_stat(std::array<unsigned int, 6>& stats) { stats = stat; }

void Player::use_effect() {
	for (int i = effects.size() - 1; i >= 0; i--) {
		unsigned int type = effects[i].stat_changed;
		int difference = effects[i].stat_difference;
		int cur_stat = get_stat(type);

		if (effects[i].change_turns < 1) {
			if (type > 1 && type < 6 && effects[i].effect_applied)
				set_stat(type, cur_stat - difference);
			effects.pop_back();
			continue;
		}

		if (type > 5 && cur_stat + difference <= get_stat(type % 6))
			set_stat(type, cur_stat + difference);
		else if (type > 1 && type < 6 && effects[i].change_turns == effects[i].original_turns
			&& cur_stat + difference > 0 && cur_stat + difference < INT_MAX) {
			set_stat(type, cur_stat + difference);
			effects[i].effect_applied = true;
		}

		effects[i].change_turns--;
	}
}

void Player::reset_effect() {
	for (int i = effects.size() - 1; i >= 0; i--) {
		unsigned int type = effects[i].stat_changed;
		int difference = effects[i].stat_difference;
		int cur_stat = get_stat(type);

		if (type > 1 && type < 6 && effects[i].effect_applied)
			set_stat(type, cur_stat - difference);
	}

	effects.clear();
}

std::vector<Effect> Player::get_effects() {
	return effects;
}