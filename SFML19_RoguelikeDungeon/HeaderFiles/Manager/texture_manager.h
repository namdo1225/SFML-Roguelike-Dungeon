/**
*
* File: texture_manager.h
* Description: Contain the declaration of the Texture_Manager class, which represents a class that manages the game's textures and sprites.
*
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Texture_Manager {
public:
	static bool assets_loaded;
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
	*	a bool. true if all assets are successfully loaded.
	*/
	static bool load();
};

#endif