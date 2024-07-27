/**
*
* File: audio_manager.h
* Description: Contain the declaration of the Audio_Manager class, which represents a class that manages the game's sounds and music.
*
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <array>

class Audio_Manager {
private:
	static std::array<sf::SoundBuffer, 30> buffers;
	static std::array<sf::Sound, 30> sounds;
	static sf::Music music;

	static bool assets_loaded;
	static float sfx_volume;

	/**
	* Constructor for Audio_Manager.
	*/
	Audio_Manager();

public:

	/**
	* Method to load sounds and music.
	*
	* Return:
	*	a bool. true if all assets are successfully loaded.
	*/
	static bool load();
	
	/**
	* Set SFXs' volume.
	*
	* Parameter:
	*	volume: a float for the SFX's new volume.
	*/
	static void set_sfx_volume(float volume);

	/**
	* Set all musics' volume.
	*
	* Parameter:
	*	volume: a float for the musics' new volume.
	*/
	static void set_music_volume(float volume);

	/**
	* Getter for SFXs' volume.
	*
	* Return:
	*	a float for the SFXs' volume.
	*/
	static float get_sfx_volume();

	/**
	* Getter for musics' volume.
	*
	* Return:
	*	a float for the musics' volume.
	*/
	static float get_music_volume();

	/**
	* Plays an SFX.
	*
	* Parameter:
	*	id: an unsigned int for which SFX to play.
	*/
	static void play_sfx(unsigned int id);

	/**
	* Plays music.
	*
	* Parameter:
	*	id: an unsigned int for which SFX to play.
	*/
	static void play_music(unsigned int id);
};


#endif