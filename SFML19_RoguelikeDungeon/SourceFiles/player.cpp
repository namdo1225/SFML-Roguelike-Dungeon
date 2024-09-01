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

void Player::setEffect(Stat stat, int quantity, unsigned int longevity, unsigned int current) {
	if (current == 0)
		effects.push_back(Effect(stat, quantity, longevity));
	else {
		effects.push_back(Effect(stat, quantity, longevity, current));
		if (current != longevity)
			effects[effects.size() - 1].effect_applied = true;
	}
}

int Player::hurtPlayer(Attack type, int quantity) {
	int finalQuantity{ 1 };
	Stat protection = type ? Def : Res;

	if (stat[protection] < quantity && quantity > 0)
		finalQuantity = quantity - stat[protection];
	stat[Hp] -= finalQuantity;

	return finalQuantity;
}

void Player::useMP(int quantity) { stat[Mp] -= quantity; }

long Player::getStat(Stat stat) { return this->stat[stat]; }

int Player::getPos(char z) { return (z == 'x') ? getPosition().x : getPosition().y; }

bool Player::useGold(unsigned int quantity) {
	if (quantity > gold)
		return false;
	gold -= quantity;
	return true;
}

std::string Player::getName() { return name; }

unsigned int Player::getLVL() { return level; }

unsigned int Player::getCurEXP() { return curEXP; }

unsigned int Player::getLVLUpEXP() { return lvlUpEXP; }

unsigned int Player::getGold() { return gold; }

unsigned int Player::getStatPts() { return points; }

unsigned int Player::getMaxItems() { return maxItem; }

unsigned int Player::getFloor() { return floor; }

void Player::setName(std::string name) { this->name = name; }

void Player::setFloor(unsigned int num) { floor = (num % INT_MAX) == 0 ? 1 : num; }

void Player::setLVL(unsigned int lvl) { level = lvl; }

void Player::setGold(unsigned int gd) { gold = gd; }

void Player::setStatPoint(unsigned int pts) { points = pts; }

void Player::setCurEXP(unsigned int curr_exp) { curEXP = curr_exp; }

void Player::setLVLUpEXP(unsigned int lvl_exp) { lvlUpEXP = lvl_exp; }

void Player::setMaxItem(unsigned int slots) { maxItem = slots; }

void Player::setStat(Stat type, unsigned int num) { stat[type] = num; }

bool Player::isDead() { return (stat[Hp] <= 0); }

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
	curEXP = 0;
	gold = cheat ? 10000 : STARTING_GOLD;
	lvlUpEXP = 10;
}

bool Player::isStuck(unsigned int i) { return stuck[i]; }

void Player::setPos(int x, int y) { setPosition(x, y); }

void Player::setStuck(unsigned int i, bool j) { stuck[i] = j; }

void Player::copyStat(std::array<long, 8>& stats) { stats = stat; }

void Player::useEffect() {
	for (int i = effects.size() - 1; i >= 0; i--) {
		Stat type = effects[i].stat_changed;
		int difference = effects[i].stat_difference;
		long cur_stat = getStat(type);

		if (effects[i].change_turns < 1) {
			if (type > Max_Mp && type < Hp && effects[i].effect_applied)
				setStat(type, cur_stat - difference);
			effects.erase(effects.begin() + i);
			continue;
		}

		if (type > Res) {
			long new_quantity = cur_stat + difference;
			long max = getStat(type == Hp ? Max_Hp : Max_Mp);
			if (new_quantity <= 0)
				setStat(type, 1);
			else if (max <= new_quantity)
				setStat(type, max);
			else
				setStat(type, new_quantity);
		}
		else if (type > Max_Mp && type < Hp && effects[i].change_turns == effects[i].original_turns
			&& cur_stat + difference > 0 && cur_stat + difference < INT_MAX) {
			setStat(type, cur_stat + difference);
			effects[i].effect_applied = true;
		}
		effects[i].change_turns--;
	}
}

void Player::resetEffect() {
	effects.clear();
}

std::vector<Effect> Player::getEffect() {
	return effects;
}

sf::FloatRect Player::getRect() {
	return getGlobalBounds();
}
