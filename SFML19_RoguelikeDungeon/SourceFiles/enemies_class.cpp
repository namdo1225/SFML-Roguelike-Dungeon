/**
*
* File: enemies_class.cpp
* Description: Contains implementations of Enemy class.
*
*/

#include "enemies_class.h"

const unsigned int Enemy::num_textures;
std::array<sf::Texture, Enemy::num_textures> Enemy::texs = { sf::Texture(), sf::Texture(), sf::Texture(), sf::Texture() };

Enemy::Enemy(int hp, int floor, unsigned int t_id, int x, int y) : id(t_id) {
	setSize(sf::Vector2f(40, 40));
	setTextureRect(sf::IntRect(0, 0, 40, 40));

	setPosition(x, y);

	switch (t_id) {
	case 0:
		// Zombie
		set_stat_h(9 + floor, 3 + (floor * 0.5), 3 + (floor * 0.5), 1 + (floor * 0.25), 1, 3 + (floor * 0.75));
		break;
	case 1:
		// Skeleton
		set_stat_h(7 + floor, 2 + (floor * 0.25), 2 + (floor * 0.25), 3 + (floor * 0.5), 3, 3 + (floor * 0.75));
		break;
	case 2:
		// Mage
		type = false;
		set_stat_h(3 + floor, 4 + (floor * 0.5), 1 + (floor * 0.125), 5 + (floor * 0.5), 4, 3 + (floor * 0.75));
		break;
	case 3:
		// Bandit
		set_stat_h(12 + floor, 4 + (floor * 0.25), 5 + (floor * 0.25), 4 + (floor * 0.25), 2, 5 + (floor * 1.5));
		break;
	}

	setTexture(&texs[t_id], false);

	// It means that the enemies aren't being added from a save file; use default hp.
	if (hp != -1)
		stats[0] = hp;
}

void Enemy::set_stat_h(int hp, int atk, int def, int res, int range, int exp) {
	stats = {hp, atk, def, res, range, exp};
}

int Enemy::get_pos(char z) { return z == 'x' ? getPosition().x : getPosition().y; }

int Enemy::set_hp(int atk_type, int amount) {
	int stat{ atk_type == 2 ? stats[2] : stats[3] };
	int quantity{ stat >= amount ? 1 : amount - stat };
	stats[0] -= quantity;
	return quantity;
}

unsigned int Enemy::get_id() { return id; }

void Enemy::set_pos(int x, int y) { setPosition(x, y); }

int Enemy::get_stat(unsigned int i) { return stats[i]; }

bool Enemy::get_type() { return type; }

void Enemy::draw(sf::RenderWindow& window) { window.draw(*this); }

void Enemy::set_stat(unsigned int i, int j) { stats[i] = j; }

bool Enemy::load_texture() {
	for (int i = 0; i < num_textures; i++) {
		bool loaded = true;
		switch (i) {
		case 0:
			// Zombie
			loaded = texs[i].loadFromFile("Texture\\Enemies\\00_Zombie.png");
			break;
		case 1:
			// Skeleton
			loaded = texs[i].loadFromFile("Texture\\Enemies\\01_Skeleton.png");
			break;
		case 2:
			// Mage
			loaded = texs[i].loadFromFile("Texture\\Enemies\\02_Mage.png");
			break;
		default:
			// Bandit
			loaded = texs[i].loadFromFile("Texture\\Enemies\\03_Bandit.png");
			break;
		}

		if (!loaded)
			return false;
	}
	return true;
}