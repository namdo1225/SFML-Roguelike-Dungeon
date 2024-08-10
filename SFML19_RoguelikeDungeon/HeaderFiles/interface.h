/**
* 
* File: interface.h
* Description: This class controls the entire game essentially, creating the interface
* and connect all other classes together. This class uses the Singleton pattern.
*
*/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "Screen/screen.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Interface {
protected:
	/**
	* Constructor for Interface.
	*/
	Interface();

	/**
	* Deconstructor for Interface.
	*/
	~Interface();

	static Interface* singleton;

private:
	/**
	* Prevent instance from being copied.
	*/
	Interface(Interface& dup);

	/**
	* Prevent the use of operator= with this class instance.
	*/
	void operator=(const Interface&);

public:
	sf::RenderWindow& window = Screen::window;

	static Interface& get();

	// handle actual events
	/**
	* The main method which contains the main loop for the game.
	*/
	void window_loop();

	/**
	* Draw the UI of the game.
	*/
	void draw_interface();

	/**
	* Has a loop to handle the events of the game.
	*/
	void handle_event();
};

#endif