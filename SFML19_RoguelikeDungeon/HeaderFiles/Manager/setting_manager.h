/**
*
* File: setting_manager.h
* Description: Contain the declaration of the Setting_Manager class, which represents a class that manages the game's settings.
*
*/

#ifndef SETTING_H
#define SETTING_H

#include <string>

class Setting_Manager {
protected:
	/**
	* Constructor for Setting_Manager.
	*/
	Setting_Manager();

public:
	const static unsigned int themes = 2;
	static unsigned int theme;
	static unsigned int sfx_volume;
	static unsigned int music_volume;
	static unsigned int font;
	static bool light;

	/**
	* Method to load settings from settings.json.
	*
	* Return:
	*	a bool. true if all settings are successfully loaded.
	*/
	static bool load();

	/**
	* Method to save settings to settings.json.
	*
	* Parameter:
	*	create: true if a new setting file needs to be created.
	*/
	static bool save(bool create = false);

};


#endif