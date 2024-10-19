#include <string>
#ifndef SETTING_H
#define SETTING_H

/**
* A class that manages the game's settings.
*/
class Setting_Manager {
protected:
	/**
	* Constructor for Setting_Manager.
	*/
	Setting_Manager();

public:
	const static unsigned int THEMES = 2;
	static unsigned int theme;
	static unsigned int sfxVolume;
	static unsigned int musicVolume;
	static unsigned int font;
	static bool light;
	static std::string saveLocation;

	/**
	* Method to load settings from settings.json.
	*
	* Return:
	*	true if all settings are successfully loaded.
	*/
	static bool load();

	/**
	* Method to save settings to settings.json.
	*
	* Parameter:
	*	create: true if a new setting file needs to be created.
	* 
	* Return:
	*	true if all settings are successfully saved.
	*/
	static bool save(bool create = false);

};


#endif