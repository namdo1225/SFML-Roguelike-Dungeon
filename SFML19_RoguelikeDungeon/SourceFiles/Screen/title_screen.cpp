/**
*
* File: title_screen.cpp
* Description: Contain the implementation of the Title_Screen class.
*/


#include "Manager/audio_manager.h"
#include "Screen/title_screen.h"
#include <cstdio>
#include <cstdlib>
#include <format>
#include <Manager/game_manager.h>
#include <Manager/setting_manager.h>
#include <Screen/screen.h>
#include <format>

// https://stackoverflow.com/questions/3037088/how-to-open-the-default-web-browser-in-windows-in-c
#if defined(_WIN32)
const char* OS = "Windows";
const char* OPEN_BROWSER = "start";
#elif defined(__APPLE__)
const char* OS = "Mac OS";
const char* OPEN_BROWSER = "open";
#elif defined(__linux__)
const char* OS = "Linux";
const char* OPEN_BROWSER = "xdg-open";
#elif defined(__unix__) 
const char* OS = "Unix";
const char* OPEN_BROWSER = "xdg-open";
#else
const char* OS = "Other";
const char* OPEN_BROWSER = "";
#endif

Title_Screen::Title_Screen() : Screen() {
	setup_helper(NULL, 410.f, 475.f, 150.f,  65.f);
	setup_helper(NULL, 700.f,  10.f, 470.f, 780.f);

	setupTextbox("New Game", 250.f, 325.f, 150.f, 65.f, []() {
		Game_Manager::reset_game();
		switch_screen(TitleScreen, NameScreen, false);
	});
	setupTextbox("Cheat Mode", 410.f, 325.f, 150.f, 65.f, []() {
		Game_Manager::reset_game(true);
		switch_screen(TitleScreen, NameScreen, false);
	});
	setupTextbox("Load Game", 330.f, 400.f, 150.f, 65.f, []() {
		switch_screen(TitleScreen, LoadScreen, true);
	});
	setupTextbox("Settings", 250.f, 475.f, 150.f, 65.f, []() {
		switch_screen(TitleScreen, SettingScreen, true);
	});
	setup_helper(std::format("Music: {}",
		Audio_Manager::get_music_volume() ? "ON" : "OFF").c_str(),
		420.f, 490.f, NULL, NULL);
	setupTextbox("Mod Game", 330.f, 550.f, 150.f, 65.f, []() { switch_screen(TitleScreen, CustomScreen, true); });
	setupTextbox("Repository", 330.f, 625.f, 150.f, 65.f, []() {
		if (OS != "Other")
			system(std::format("{} https://github.com/namdo1225/SFML-Roguelike-Dungeon", OPEN_BROWSER).c_str());
		else
			printf("We cannot open the repository with your OS.");
	});
	setupTextbox("Quit Game", 330.f, 700.f, 150.f, 65.f, []() {
		window.close();
	});

	setup_helper("Roguelike\nDungeon", 170.f, 40.f, 96.f, NULL);

	setup_helper(
		"Developed with the help of internet forums\nand SFML Graphics, zlib/png license."
		"\n\nFonts: fontsource.org, Google Fonts\n\nInstructions: use the arrow keys to "
		"move the\nplayers. Tap on the level up button\nto upgrade your stats."
		"\n\nSound effects created on sfxr.me, MIT License."
		"\n\nSongs created on beepbox.co, MIT License."
		"\n\nSprites created in Krita."
		"\n\nJSON parser: github.com/nlohmann/json."
		"\n\Candle: github.com/Denvi/Candle."
		"\n\Thor: bromeon.ch/libraries/thor/."
		"\n\SQLite3: sqlite.org."
		"\n\NativeFileDialog:"
		"\ngithub.com / mlabbe / nativefiledialog."
		"\n\nVersion: 5.0, major overhaul.",
		710.f, 20.f, 18.f, NULL);
}

void Title_Screen::click_event_handler() {
	if (mouse_in_helper(true, 0)) {
		Audio_Manager::set_music_volume(Audio_Manager::get_music_volume() ? 0 : 100);
		const float new_volume = Audio_Manager::get_music_volume();
		Setting_Manager::music_volume = new_volume;
		Setting_Manager::save();
		texts[0].setString(std::format("Music: {}", new_volume == 0 ? "OFF" : "ON").c_str());
	}
}

void Title_Screen::hover_event_handler() {
	hover_textbox(0, 0);
}