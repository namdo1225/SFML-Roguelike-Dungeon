/**
*
* File: enemies_class.cpp
* Description: Contains implementations of Enemy class.
*
*/

#include "enemies_class.h"

sf::Texture Enemy::en_tex;

Enemy::Enemy(int hp, int floor, unsigned int t_id, int x, int y) : id(t_id) {
	rect.setSize(sf::Vector2f(40, 40));
	rect.setFillColor(sf::Color::Magenta);
	rect.setTexture(&en_tex);
	rect.setTextureRect(sf::IntRect(0, 0, 40, 40));

	rect.setPosition(x, y);

	switch (t_id) {
	case 1:
		// Zombie
		set_stat_h(9 + floor, 3 + (floor * 0.5), 3 + (floor * 0.5), 1 + (floor * 0.25), 1, 3 + (floor * 0.75));
		break;
	case 2:
		// Skeleton
		set_stat_h(7 + floor, 2 + (floor * 0.25), 2 + (floor * 0.25), 3 + (floor * 0.5), 3, 3 + (floor * 0.75));
		break;
	case 3:
		// Mage
		type = false;
		set_stat_h(3 + floor, 4 + (floor * 0.5), 1 + (floor * 0.125), 5 + (floor * 0.5), 4, 3 + (floor * 0.75));
		break;
	case 4:
		// Bandit
		set_stat_h(12 + floor, 4 + (floor * 0.25), 5 + (floor * 0.25), 4 + (floor * 0.25), 2, 5 + (floor * 1.5));
		break;
	}

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

int Enemy::get_pos(char z) { return z == 'x' ? rect.getPosition().x : rect.getPosition().y; }

int Enemy::set_hp(int atk_type, int amount) {
	int stat{ atk_type == 2 ? stats[2] : stats[3] };
	int quantity{ stat >= amount ? 1 : amount - stat };
	stats[0] -= quantity;
	return quantity;
}

unsigned int Enemy::get_id() { return id; }

void Enemy::set_pos(int x, int y) { rect.setPosition(x, y); }

int Enemy::get_stat(unsigned int i) { return stats[i]; }

bool Enemy::get_type() { return type; }

void Enemy::draw(sf::RenderWindow& window) { window.draw(rect); }

void Enemy::set_stat(unsigned int i, int j) { stats[i] = j; }

bool Enemy::load_texture() {
	if (!en_tex.loadFromFile("Texture\\GG_03_Enemies.png"))
		return false;
	return true;
}