#include <array>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#ifndef AUDIO_H
#define AUDIO_H

/**
* Manages the game's sounds and music.
*/
class Audio_Manager {
private:
	static const unsigned int SOUNDS = 5;
	static const unsigned int MUSICS = 1;

	static std::array<sf::SoundBuffer, SOUNDS> buffers;
	static std::array<sf::Sound, SOUNDS> sounds;
	static std::array <sf::Music, MUSICS> musics;

	static bool loaded;
	static float sfxVolume;
	static float musicVolume;

	/**
	* Constructor for Audio_Manager.
	*/
	Audio_Manager();

public:

	/**
	* Method to load sounds and music.
	*
	* Return:
	*	true if all assets are successfully loaded.
	*/
	static bool load();
	
	/**
	* Set SFXs' volume.
	*
	* Parameter:
	*	volume: the SFXs' new volume.
	*/
	static void setSFXVolume(float volume);

	/**
	* Set all musics' volume.
	*
	* Parameter:
	*	volume: the musics' new volume.
	*/
	static void setMusicVolume(float volume);

	/**
	* Getter for SFXs' volume.
	*
	* Return:
	*	the SFXs' volume.
	*/
	static float getSFXVolume();

	/**
	* Getter for musics' volume.
	*
	* Return:
	*	the musics' volume.
	*/
	static float getMusicVolume();

	/**
	* Plays an SFX.
	*
	* Parameter:
	*	id: the id of the SFX to play.
	*/
	static void playSFX(unsigned int id);

	/**
	* Plays music.
	*
	* Parameter:
	*	id: the id of the music to play.
	*/
	static void playMusic(unsigned int id);
};


#endif