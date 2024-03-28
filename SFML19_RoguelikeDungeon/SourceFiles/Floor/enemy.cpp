/**
*
* File: enemy.cpp
* Description: Contains implementations of Enemy class.
*
*/

#include "Floor/enemy.h"
#include "Manager/texture_manager.h"
#include "Manager/sf_manager.h"

Enemy::Enemy(int hp, int floor, unsigned int id, float x, float y) : Floor_Object(x, y, Texture_Manager::collectible) {
	this->id = id;
	switch (id) {
	case 0:
		name = "Zombie";
		set_stat_h(9 + floor, 3 + (floor * 0.5), 3 + (floor * 0.5), 1 + (floor * 0.25), 1, 3 + (floor * 0.75));
		break;
	case 1:
		name = "Skeleton";
		set_stat_h(7 + floor, 2 + (floor * 0.25), 2 + (floor * 0.25), 2 + (floor * 0.5), 3, 3 + (floor * 0.75));
		break;
	case 2:
		name = "Mage";
		type = false;
		set_stat_h(3 + floor, 4 + (floor * 0.5), 1 + (floor * 0.125), 4 + (floor * 0.5), 4, 3 + (floor * 0.75));
		break;
	case 3:
		name = "Bandit";
		set_stat_h(12 + floor, 4 + (floor * 0.25), 5 + (floor * 0.25), 3 + (floor * 0.25), 2, 5 + (floor * 1.5));
		break;
	}

	setTexture(&Texture_Manager::tex_enemies[id], false);

	// It means that the enemies aren't being added from a save file; use default hp.
	if (hp != -1)
		stats[0] = hp;
}

void Enemy::set_stat_h(int hp, int atk, int def, int res, int range, int exp) {
	stats[0] = hp;
	stats[1] = atk;
	stats[2] = def;
	stats[3] = res;
	stats[4] = range;
	stats[5] = exp;
}

int Enemy::set_hp(int atk_type, int amount) {
	int stat{ atk_type == 2 ? stats[2] : stats[3] };
	int quantity{ stat >= amount ? 1 : amount - stat };
	stats[0] -= quantity;
	return quantity;
}

unsigned int Enemy::get_id() { return id; }

int Enemy::get_stat(unsigned int i) { return stats[i]; }

bool Enemy::get_type() { return type; }

std::string Enemy::get_name() {
	return name;
}

void Enemy::set_stat(unsigned int i, int j) { stats[i] = j; }