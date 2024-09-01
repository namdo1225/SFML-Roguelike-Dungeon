#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

#ifndef TEXTURE_H
#define TEXTURE_H

/**
* A class that manages the game's textures and sprites.
*/
class Texture_Manager {
public:
	static bool loaded;
	static std::map<unsigned int, sf::Texture> tex_enemies;
	static std::vector<sf::Texture> tex_rooms;

	static sf::Texture gold;
	static sf::Texture player;
	static sf::Texture collectible;
	static sf::Texture stair;
	static sf::Texture shop;
	static sf::Texture interactible;
	static sf::Texture interactibleInactive;

	/**
	* Constructor for Texture_Manager.
	*/
	Texture_Manager();

	/**
	* Method to load textures.
	*
	* Return:
	*	true if all assets are successfully loaded.
	*/
	static bool load();
};

#endif