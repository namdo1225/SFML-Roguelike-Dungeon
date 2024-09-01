/**
*
* File: texture_manager.cpp
* Description: Contains the implementation of the Texture_Manager class.
*
*/

#include "Manager/texture_manager.h"
#include "util.h"
#include <filesystem>
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>

bool Texture_Manager::loaded = false;

std::map<unsigned int, sf::Texture> Texture_Manager::tex_enemies;
std::vector<sf::Texture> Texture_Manager::tex_rooms;

sf::Texture Texture_Manager::gold;
sf::Texture Texture_Manager::player;
sf::Texture Texture_Manager::collectible;
sf::Texture Texture_Manager::stair;
sf::Texture Texture_Manager::shop;
sf::Texture Texture_Manager::interactible;
sf::Texture Texture_Manager::interactibleInactive;

Texture_Manager::Texture_Manager() {
	if (!loaded)
		load();
}

bool Texture_Manager::load() {
	if (loaded)
		return true;


	for (auto& file : std::filesystem::directory_iterator("Texture\\Enemies")) {
		const std::string path = file.path().string();
		auto splits = Util::split(path, "\\");
		std::string filename = splits[splits.size() - 1];
		auto splits2 = Util::split(filename, "_");
		unsigned int id = std::stoi(splits2[0]);

		if (!tex_enemies[id].loadFromFile(path))
			return false;
	}

	for (auto& file : std::filesystem::directory_iterator("Texture\\Paths")) {
		tex_rooms.push_back(sf::Texture());
		unsigned int i = tex_rooms.size() - 1;
		if (!tex_rooms[i].loadFromFile(file.path().string()))
			return false;

		tex_rooms[i].setRepeated(true);
	}

	if (gold.loadFromFile("Texture\\GG_01_Gold.png") &&
		player.loadFromFile("Texture\\GG_02_Player.png") &&
		collectible.loadFromFile("Texture\\GG_04_Items.png") &&
		stair.loadFromFile("Texture\\GG_05_Stair.png") &&
		shop.loadFromFile("Texture\\GG_06_Shop.jpg") &&
		interactible.loadFromFile("Texture\\GG_10_Interactible.jpg") &&
		interactibleInactive.loadFromFile("Texture\\GG_11_Interactible_Inactive.jpg")) {
		loaded = true;
		return loaded;
	}

	loaded = false;
	return loaded;
}