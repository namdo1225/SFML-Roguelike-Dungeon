/**
*
* File: audio_manager.cpp
* Description: Contains the implementation of the Audio_Manager class.
*
*/

#include "Manager/audio_manager.h"
#include <array>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundSource.hpp>

bool Audio_Manager::loaded = false;
float Audio_Manager::sfxVolume = 100.f;

std::array<sf::SoundBuffer, 30> Audio_Manager::buffers;
std::array<sf::Sound, 30> Audio_Manager::sounds;
sf::Music Audio_Manager::music;

Audio_Manager::Audio_Manager() {
	if (!loaded)
		load();
}

bool Audio_Manager::load() {
	if (loaded)
		return true;

	if (music.openFromFile("Sound\\05_music.wav") &&
		buffers[0].loadFromFile("Sound\\00_item_collected.wav") &&
		buffers[1].loadFromFile("Sound\\01_stair.wav") &&
		buffers[2].loadFromFile("Sound\\02_attack.wav") &&
		buffers[3].loadFromFile("Sound\\03_shop_open.wav") &&
		buffers[4].loadFromFile("Sound\\04_item_select.wav")
		) {
		for (unsigned int i = 0; i < 5; i++)
			sounds[i].setBuffer(buffers[i]);
		loaded = true;
		return loaded;
	}

	loaded = false;
	return loaded;
}

void Audio_Manager::setSFXVolume(float volume) {
	if (volume >= 0 && volume <= 100) {
		sfxVolume = volume;
		for (unsigned int i = 0; i < 5; i++)
			sounds[i].setVolume(volume);
	}
}

void Audio_Manager::setMusicVolume(float volume) {
	if (volume >= 0 && volume <= 100)
		music.setVolume(volume);
}

float Audio_Manager::getSFXVolume() {
	return sfxVolume;
}

float Audio_Manager::getMusicVolume() {
	return music.getVolume();
}

void Audio_Manager::playSFX(unsigned int id) {
	if (!loaded)
		load();

	if (id <= 0 && id<= 30)
		sounds[id].play();
}

void Audio_Manager::playMusic(unsigned int id) {
	if (!loaded)
		load();

	if (music.Stopped || music.Paused)
		music.play();
}
