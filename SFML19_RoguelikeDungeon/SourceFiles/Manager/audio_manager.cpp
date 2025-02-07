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
#include <string>
#include <util.h>
#include <filesystem>

bool Audio_Manager::loaded = false;
float Audio_Manager::sfxVolume = 100.f;
float Audio_Manager::musicVolume = 100.f;

const unsigned int Audio_Manager::SOUNDS;
const unsigned int Audio_Manager::MUSICS;

std::array<sf::SoundBuffer, Audio_Manager::SOUNDS> Audio_Manager::buffers;
std::array<sf::Sound, Audio_Manager::SOUNDS> Audio_Manager::sounds;
std::array<sf::Music, Audio_Manager::MUSICS> Audio_Manager::musics;

Audio_Manager::Audio_Manager() {
	if (!loaded)
		load();
}

bool Audio_Manager::load() {
	if (loaded)
		return true;

	for (auto& file : std::filesystem::directory_iterator("SFX")) {
		const std::string path = file.path().string();
		auto splits = Util::split(path, "\\");
		std::string filename = splits[splits.size() - 1];
		auto splits2 = Util::split(filename, "_");
		unsigned int id = std::stoi(splits2[0]);

		if (!buffers[id].loadFromFile(path))
			return false;

		sounds[id].setBuffer(buffers[id]);
	}

	for (auto& file : std::filesystem::directory_iterator("Music")) {
		const std::string path = file.path().string();
		auto splits = Util::split(path, "\\");
		std::string filename = splits[splits.size() - 1];
		auto splits2 = Util::split(filename, "_");
		unsigned int id = std::stoi(splits2[0]);

		if (!musics[id].openFromFile(path))
			return false;
	}

	loaded = true;
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
	if (volume >= 0 && volume <= 100) {
		musicVolume = volume;
		for (unsigned int i = 0; i < 5; i++)
			musics[i].setVolume(volume);
	}
}

float Audio_Manager::getSFXVolume() {
	return sfxVolume;
}

float Audio_Manager::getMusicVolume() {
	return musicVolume;
}

void Audio_Manager::playSFX(unsigned int id) {
	if (!loaded)
		load();

	if (id >= 0 && id <= SOUNDS)
		sounds[id].play();
}

void Audio_Manager::playMusic(unsigned int id) {
	if (!loaded)
		load();

	if (id >= 0 && id <= MUSICS && (musics[id].Stopped || musics[id].Paused))
		musics[id].play();
}
