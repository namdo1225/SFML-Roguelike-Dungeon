#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics/Font.hpp>
#include <string>
#include <vector>

/**
* A class that manages the game's fonts.
*/
class Font_Manager {
private:
	static unsigned int selected_font;
	static std::vector<sf::Font> buffers;
	static bool loaded;

protected:
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

	/**
	* Gets a font.
	* 
	* Parameter:
	*	i: an unsigned int for the font number.
	* 
	* Return:
	*	the sf::Font object.
	*/
	static sf::Font& get(unsigned int i);

	/**
	* Sets a font for the game.
	* 
	* Parameter:
	*	i: an unsigned int for the font number.
	*/
	static void set(unsigned int i);

	/**
	* Gets the selected font.
	*
	* Return:
	*	the sf::Font object.
	*/
	static sf::Font& get_selected();

	static unsigned int getNumFonts();

	static std::string getFontName(unsigned int i);
};


#endif