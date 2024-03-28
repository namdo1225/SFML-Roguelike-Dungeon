/**
*
* File: audio_manager.cpp
* Description: Contains the implementation of the Audio_Manager class.
*
*/

#include "Manager/audio_manager.h"

bool Audio_Manager::assets_loaded = false;
float Audio_Manager::sfx_volume = 100.f;

std::array<sf::SoundBuffer, 30> Audio_Manager::buffers;
std::array<sf::Sound, 30> Audio_Manager::sounds;
sf::Music Audio_Manager::music;

Audio_Manager::Audio_Manager() {
	if (!assets_loaded)
		load();
}

bool Audio_Manager::load() {
	if (assets_loaded)
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
		assets_loaded = true;
		return assets_loaded;
	}

	assets_loaded = false;
	return assets_loaded;
}

void Audio_Manager::set_sfx_volume(float volume) {
	if (volume >= 0 && volume <= 100) {
		sfx_volume = volume;
		for (unsigned int i = 0; i < 5; i++)
			sounds[i].setVolume(volume);
	}
}

void Audio_Manager::set_music_volume(float volume) {
	if (volume >= 0 && volume <= 100)
		music.setVolume(volume);
}

float Audio_Manager::get_sfx_volume() {
	return sfx_volume;
}

float Audio_Manager::get_music_volume() {
	return music.getVolume();
}

void Audio_Manager::play_sfx(unsigned int id) {
	if (!assets_loaded)
		load();

	if (id <= 0 && id<= 30)
		sounds[id].play();
}

void Audio_Manager::play_music(unsigned int id) {
	if (!assets_loaded)
		load();

	if (music.Stopped || music.Paused)
		music.play();
}
