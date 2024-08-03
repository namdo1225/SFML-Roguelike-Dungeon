/**
*
* File: title_screen.cpp
* Description: Contain the implementation of the Title_Screen class.
*/


#include "Screen/title_screen.h"
#include "interface.h"
#include "Manager/audio_manager.h"
#include <format>

Title_Screen::Title_Screen() : Screen() {
	setup_helper(NULL, 330.f, 625.f, 150.f,  65.f);
	setup_helper(NULL, 700.f,  10.f, 470.f, 780.f);

	setupTextbox("New Game", 330.f, 325.f, 150.f, 65.f, []() {
		Game_Manager::reset_game();
		switch_screen(TitleScreen, NameScreen, false);
	});
	setupTextbox("Load Game", 330.f, 400.f, 150.f, 65.f, []() {
		switch_screen(TitleScreen, LoadScreen, true);
	});
	setupTextbox("Settings", 330.f, 475.f, 150.f, 65.f, []() {
		switch_screen(TitleScreen, SettingScreen, true);
	});
	setupTextbox("Quit Game", 330.f, 550.f, 150.f, 65.f, []() {
		window.close();
	});
	setup_helper(std::format("Music: {}",
		Audio_Manager::get_music_volume() ? "ON" : "OFF").c_str(),
		335.f, 643.f, NULL, NULL);
	setupTextbox("Mod Game", 330.f, 700.f, 150.f, 65.f, []() { switch_screen(TitleScreen, CustomScreen, true); });

	setup_helper("Roguelike\nDungeon", 170.f, 40.f, 96.f, NULL);

	setup_helper(
		"Developed with the help of\ninternet forums and SFML Graphics,\nzlib/png license."
		"\n\nFonts: fontsource.org, Google Fonts\n\nInstructions: use the arrow keys to "
		"\nmove the players. Tap on the level \nup button to upgrade your stats."
		"\n\nSound effects created on \nsfxr.me/, MIT License."
		"\n\nSongs created on \nbeepbox.co/, MIT License."
		"\n\nSprites created in Krita."
		"\n\nJSON parser: \nhttps://github.com/nlohmann/json."
		"\n\nVersion: 4.0, refactored UI.",
		710.f, 20.f, NULL, NULL);
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