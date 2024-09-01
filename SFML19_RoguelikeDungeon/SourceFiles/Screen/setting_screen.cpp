/**
*
* File: setting_screen.cpp
* Description: Contain the implementation of the Setting_Screen class.
*/


#include "Screen/setting_screen.h"
#include <Manager/font_manager.h>
#include <Manager/setting_manager.h>
#include <Screen/screen.h>
#include <string>
#include <Manager/database_manager.h>

bool Setting_Screen::light_mode = false;
unsigned int Setting_Screen::theme = 0;
unsigned int Setting_Screen::sfxVolume = 100;
unsigned int Setting_Screen::music_volume = 100;
unsigned int Setting_Screen::font = 0;

Setting_Screen::Setting_Screen() : Screen(true, true, true) {
	update = true;

	light_mode = Setting_Manager::light;
	theme = Setting_Manager::theme;
	sfxVolume = Setting_Manager::sfxVolume;
	music_volume = Setting_Manager::music_volume;
	font = Setting_Manager::font;

	textRectH(  "Settings", 400.f, 10.f, 96.f, NULL, false);
	textRectH(     "Theme", 20.f, 150.f, NULL, NULL, false);
	textRectH("Light Mode", 20.f, 200.f, NULL, NULL, false);
	textRectH(      "Font", 20.f, 250.f, NULL, NULL, false);
	textRectH("SFX Volume", 20.f, 300.f, NULL, NULL, false);
	textRectH(     "Music", 20.f, 350.f, NULL, NULL, false);
	textRectH("Selected", 1000.f, 100.f, NULL, NULL, false);

	for (int i = 0; i < 10; i++) {
		hoverableTextH(std::to_string(i * 10).c_str(), 300.f + (50 * i), 300.f, [this, i]() {
			sfxVolume = i * 10;
			texts[10].setString(std::to_string(sfxVolume));
		});
	}

	for (int i = 0; i < 10; i++) {
		unsigned int j = i + 10;
		hoverableTextH(std::to_string(i * 10).c_str(), 300.f + (50 * i), 350.f, [this, i]() {
			music_volume = i * 10;
			texts[11].setString(std::to_string(music_volume));
		});
	}

	for (int i = 0; i < 2; i++) {
		unsigned int j = i + 20;
		hoverableTextH(i ? "Dark" : "Light", 300.f + (100.f * i), 200.f, [this, i]() {
			light_mode = i;
			texts[8].setString(i ? "Dark" : "Light");
		});
	}

	for (int i = 0; i < THEMES; i++) {
		unsigned int j = i + 22;
		hoverableTextH(std::to_string(i).c_str(), 300.f + (100.f * i), 150.f, [this, i]() {
			theme = i;
			texts[7].setString(std::to_string(theme));
		});
	}

	for (int i = 0; i < Font_Manager::getNumFonts(); i++) {
		unsigned int j = i + 22 + THEMES;
		hoverableTextH(Font_Manager::getFontName(i).c_str(), 300.f + (100 * i), 250.f, [this, i]() {
			font = i;
			texts[9].setString(Font_Manager::getFontName(font));
		}, 12.f);
	}

	// starting with index 7
	textRectH(       std::to_string(theme).c_str(), 1000.f, 150.f, NULL, NULL, false);
	textRectH(       light_mode ? "Light" : "Dark", 1000.f, 200.f, NULL, NULL, false);
	textRectH(Font_Manager::get_selected().getInfo().family.c_str(), 1000.f, 250.f, NULL, NULL, false);
	textRectH(  std::to_string(sfxVolume).c_str(), 1000.f, 300.f, NULL, NULL, false);
	textRectH(std::to_string(music_volume).c_str(), 1000.f, 350.f, NULL, NULL, false);
	textboxH("abcdefghijklmnopqrstuvwxyz, ABCDEFGHIJKLMNOPQRSTUVWXYZ\n!@#$%%^&*()_+-=", 20.f, 500.f, 1150.f, 95.f, NULL);

	textboxH("Reset Database", 800.f, 700.f, 300.f, 50.f, []() {
		Database_Manager::resetDB();
		window.close();
	});
}

bool Setting_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		light_mode = Setting_Manager::light;
		theme = Setting_Manager::theme;
		sfxVolume = Setting_Manager::sfxVolume;
		music_volume = Setting_Manager::music_volume;
		font = Setting_Manager::font;
		texts[7].setString(std::to_string(theme));
		texts[8].setString(light_mode ? "Light" : "Dark");
		texts[9].setString(Font_Manager::get_selected().getInfo().family);
		texts[10].setString(std::to_string(sfxVolume));
		texts[11].setString(std::to_string(music_volume));

		goToPrevScreen(SettingScreen);
		return true;
	}
	else if (mouseInButton(ConfirmButton) && (Setting_Manager::light != light_mode ||
		Setting_Manager::theme != theme || Setting_Manager::sfxVolume != sfxVolume ||
		Setting_Manager::music_volume != music_volume || Setting_Manager::font != font)) {
		Setting_Manager::light = light_mode;
		Setting_Manager::theme = theme;
		Setting_Manager::sfxVolume = sfxVolume;
		Setting_Manager::music_volume = music_volume;
		Setting_Manager::font = font;
		change_settings();
	}
}

void Setting_Screen::updateDraw() {
	texts[11].setString(std::to_string(Setting_Manager::music_volume));
	update = false;
}
