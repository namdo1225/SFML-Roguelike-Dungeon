/**
*
* File: title_screen.cpp
* Description: Contain the implementation of the Title_Screen class.
*/


#include "Screen/title_screen.h"
#include "interface.h"
#include "Manager/audio_manager.h"
#include <format>

Title_Screen::Title_Screen() : Screen(7, 6) {
	setup_helper(true, 0, NULL, 330.f, 325.f, 150.f,  65.f);
	setup_helper(true, 1, NULL, 330.f, 400.f, 150.f,  65.f);
	setup_helper(true, 2, NULL, 330.f, 475.f, 150.f,  65.f);
	setup_helper(true, 3, NULL, 330.f, 550.f, 150.f,  65.f);
	setup_helper(true, 4, NULL, 330.f, 625.f, 150.f,  65.f);
	setup_helper(true, 5, NULL, 700.f,  10.f, 470.f, 780.f);

	setup_helper(false, 0, "New Game",   335.f, 343.f, NULL, NULL);
	setup_helper(false, 1, "Load Game",  335.f, 423.f, NULL, NULL);
	setup_helper(false, 2, "Settings",   345.f, 493.f, NULL, NULL);
	setup_helper(false, 3, "Quit Game",  335.f, 568.f, NULL, NULL);
	setup_helper(false, 4, std::format("Music: {}",
		Audio_Manager::get_music_volume() == 0 ? "OFF" : "ON").c_str(),
		335.f, 643.f, NULL, NULL);

	setup_helper(false, 5, "Roguelike\nDungeon", 170.f, 40.f, 96.f, NULL);

	setup_helper(false, 6, 
		"Developed with the help of\ninternet forums and SFML Graphics,\nzlib/png license."
		"\n\nFonts: fontsource.org, Google Fonts\n\nInstructions: use the arrow keys to "
		"\nmove the players. Tap on the level \nup button to upgrade your stats."
		"\n\nSound effects created on \nsfxr.me/, MIT License."
		"\n\nSongs created on \nbeepbox.co/, MIT License."
		"\n\nSprites created in Krita."
		"\n\nJSON parser: \nhttps://github.com/nlohmann/json."
		"\n\nVersion: 4.0, refactored UI.",
		710.f, 20.f, NULL, NULL);
	rects[5].setThemeAndHover(Setting_Manager::theme, false);
	texts[5].setThemeAndHover(Setting_Manager::theme, false);
	texts[6].setThemeAndHover(Setting_Manager::theme, false);
}

void Title_Screen::click_event_handler() {
	if (mouse_in_helper(true, 3))
		window.close();
	else if (mouse_in_helper(true, 0)) {
		Game_Manager::reset_game();
		switch_screen(TitleScreen, NameScreen, false);
	}
	else if (mouse_in_helper(true, 1))
		switch_screen(TitleScreen, LoadScreen, true);
	else if (mouse_in_helper(true, 2))
		switch_screen(TitleScreen, SettingScreen, true);
	else if (mouse_in_helper(true, 4)) {
		Audio_Manager::set_music_volume(Audio_Manager::get_music_volume() ? 0 : 100);
		const float new_volume = Audio_Manager::get_music_volume();
		Setting_Manager::music_volume = new_volume;
		Setting_Manager::save();
		texts[4].setString(std::format("Music: {}", new_volume == 0 ? "OFF" : "ON").c_str());
	}
}

void Title_Screen::hover_event_handler() {
	for (unsigned int i = 0; i < 5; i++)
		hover_textbox(i, i);
}