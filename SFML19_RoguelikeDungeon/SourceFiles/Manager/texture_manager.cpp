/**
*
* File: texture_manager.cpp
* Description: Contains the implementation of the Texture_Manager class.
*
*/

#include "Manager/texture_manager.h"

bool Texture_Manager::assets_loaded = false;

const unsigned int Texture_Manager::num_enemies;
std::array<sf::Texture, Texture_Manager::num_enemies> Texture_Manager::tex_enemies = { sf::Texture(), sf::Texture(), sf::Texture(), sf::Texture() };

const unsigned int Texture_Manager::num_rooms;
std::array<sf::Texture, Texture_Manager::num_rooms> Texture_Manager::tex_rooms = { sf::Texture(), sf::Texture(), sf::Texture(), sf::Texture() };

sf::Texture Texture_Manager::gold;
sf::Texture Texture_Manager::player;
sf::Texture Texture_Manager::collectible;
sf::Texture Texture_Manager::stair;
sf::Texture Texture_Manager::shop;
sf::Texture Texture_Manager::interactible;
sf::Texture Texture_Manager::interactibleInactive;

Texture_Manager::Texture_Manager() {
	if (!assets_loaded)
		load();
}

bool Texture_Manager::load() {
	if (assets_loaded)
		return true;

	for (unsigned int i = 0; i < num_enemies; i++) {
		switch (i) {
		case 0:
			// Zombie
			assets_loaded = tex_enemies[i].loadFromFile("Texture\\Enemies\\0000_Zombie.png");
			break;
		case 1:
			// Skeleton
			assets_loaded = tex_enemies[i].loadFromFile("Texture\\Enemies\\0001_Skeleton.png");
			break;
		case 2:
			// Mage
			assets_loaded = tex_enemies[i].loadFromFile("Texture\\Enemies\\0002_Mage.png");
			break;
		default:
			// Bandit
			assets_loaded = tex_enemies[i].loadFromFile("Texture\\Enemies\\0003_Bandit.png");
			break;
		}

		if (!assets_loaded)
			return assets_loaded;
	}

	for (unsigned int i = 0; i < num_rooms; i++) {
		tex_rooms[i].setRepeated(true);
		switch (i) {
		case 0:
			assets_loaded = tex_rooms[i].loadFromFile("Texture\\Paths\\00_Stone.jpg");
			break;
		case 1:
			assets_loaded = tex_rooms[i].loadFromFile("Texture\\Paths\\01_Wood.jpg");
			break;
		case 2:
			assets_loaded = tex_rooms[i].loadFromFile("Texture\\Paths\\02_Grass.jpg");
			break;
		default:
			assets_loaded = tex_rooms[i].loadFromFile("Texture\\Paths\\03_Lava.jpg");
			break;
		}

		if (!assets_loaded)
			return assets_loaded;
	}

	if (gold.loadFromFile("Texture\\GG_01_Gold.png") &&
		player.loadFromFile("Texture\\GG_02_Player.png") &&
		collectible.loadFromFile("Texture\\GG_04_Items.png") &&
		stair.loadFromFile("Texture\\GG_05_Stair.png") &&
		shop.loadFromFile("Texture\\GG_06_Shop.jpg") &&
		interactible.loadFromFile("Texture\\GG_10_Interactible.jpg") &&
		interactibleInactive.loadFromFile("Texture\\GG_11_Interactible_Inactive.jpg")) {
		assets_loaded = true;
		return assets_loaded;
	}

	assets_loaded = false;
	return assets_loaded;
}