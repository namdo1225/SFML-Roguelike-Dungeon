/**
*
* File: spell.cpp
* Description: Contains implementations of Spell class and its sub-classes.
*
*/

#include "spell.h"

Spell::Spell() {
	icon.setFillColor(sf::Color::White);
	icon.setCharacterSize(48);

	desc.setFillColor(sf::Color::White);
	desc.setCharacterSize(24);
	desc.setPosition(760, 150);

	txt.setOutlineColor(sf::Color::White);
	txt.setFillColor(sf::Color::White);
	txt.setPosition(-100, -100);
	txt.setCharacterSize(48);
}

Spell::Spell(sf::Font& font, const char abbre[3], unsigned int t_id, unsigned int t_buy, unsigned int t_sell, unsigned int t_use) : Spell() {
	id = t_id;
	buy_gd = t_buy;
	sell_gd = t_sell;
	uses = t_use;

	icon.setFont(font);
	desc.setFont(font);
	txt.setFont(font);

	icon.setString(abbre);
	txt.setString(abbre);

	txt.setPosition(1045, 310);
}

void Spell::set_pos(int x, int y) { icon.setPosition(x, y); }

void Spell::use(Floor& floor, Player& player, std::vector<Enemy>& enemies) {}

std::array<int, 4> Spell::atk(Player& player) { return {0,0,0,0}; };

unsigned int Spell::get_id() { return id; }

unsigned int Spell::get_use() { return uses; }

int Spell::get_pos(char z) { return (z == 'x') ? icon.getPosition().x : icon.getPosition().y; }

unsigned int Spell::get_sell_gd() { return sell_gd; }

unsigned int Spell::get_buy_gd() { return buy_gd; }

Spell* Spell::create_spell(unsigned int id, sf::Font& font) {
	switch (id) {
	case 1:
		return new Heal(font);
	case 2:
		return new Damage_All(font);
	case 3:
		return new Fire(font);
	}
}

void Spell::draw(sf::RenderWindow& window, char type) {
	if (type == 't')
		window.draw(icon);
	else if (type == 'd')
		window.draw(desc);
	else
		window.draw(txt);
}


PH_Spell::PH_Spell() { id = 0; }

Heal::Heal(sf::Font& font) : Spell(font, "HE", 1, 50, 20, 3) {
	desc.setString("Heal\n\nHeal the player.\n\nPLAYER HP+: 5 + (MAG STAT * 0.5)\n\n-3MP\n\nBUY: 50G\nSELL: 20G");
}

void Heal::use(Floor& floor, Player& player, std::vector<Enemy>& enemies) {
	if (player.get_stat(Hp) == player.get_stat(Max_Hp))
		return;

	player.set_stat(Mp, player.get_stat(Mp) - 3);

	if (player.get_stat(Hp) + 5 + player.get_stat(Mgk) > player.get_stat(Max_Hp)) {
		player.set_stat(Hp, player.get_stat(Hp) + player.get_stat(Max_Hp));
		return;
	}

	player.set_stat(Hp, player.get_stat(Hp) + 5 + player.get_stat(Mgk));
}

Damage_All::Damage_All(sf::Font& font) : Spell(font, "DA", 2, 100, 40, 1) {
	desc.setString("Damage All\n\nDamage all enemies in the \nroom (ignore defense \nand resistance).\n\nENEMY HP-: 4 + (MAG STAT * 0.25)\n\n-8MP\n\nBUY: 100G\nSELL: 40G");
}

void Damage_All::use(Floor& floor, Player& player, std::vector<Enemy>& enemies) {
	if (player.get_stat(Mp) < 4)
		return;

	player.set_stat(Mp, player.get_stat(Mp) - 8);

	for (unsigned int i = 0; i < enemies.size(); i++)
		enemies[i].set_stat(0, enemies[i].get_stat(0) - 4 + player.get_stat(Mgk) * 0.25);
}

Fire::Fire(sf::Font& font) : Spell(font, "FI", 3, 10, 4, 4) {
	range = 3;
	desc.setString("Fire \n\Hit an enemy with fire 3 \nspaces away.\n\nENEMY HP-: 4HP + (MAG STAT * \n0.5) - ENEMY RES STAT\n\n-4MP\n\nBUY: 10G\nSELL: 4G");
}

std::array<int, 4> Fire::atk(Player& player) {
	std::array<int, 4> return_arr = {4 + player.get_stat(Mgk) * 0.5, range, 3, 4};
	return return_arr;
}