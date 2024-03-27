/**
*
* File: font_manager.h
* Description: Contain the declaration of the Font_Manager class, which represents a class that manages the game's fonts.
*
*/

#ifndef FONT_H
#define FONT_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Font_Manager {
private:
	const static unsigned int fonts = 3;
	static unsigned int selected_font;
	static sf::Font buffers[fonts];
	static bool assets_loaded;

	/**
	* Constructor for Font_Manager.
	*/
	Font_Manager();


public:

	/**
	* Method to load fonts.
	*
	* Return:
	*	a bool. true if all assets are successfully loaded.
	*/
	static bool load();

	static sf::Font& get(unsigned int i);

	static void set(unsigned int i);

	static sf::Font& get_selected();
};


#endif