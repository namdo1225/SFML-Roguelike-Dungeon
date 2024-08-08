/**
*
* File: setting_screen.cpp
* Description: Contain the implementation of the Setting_Screen class.
*/


#include "Screen/setting_screen.h"
#include <cstdlib>
#include <Manager/font_manager.h>
#include <Manager/setting_manager.h>
#include <Screen/screen.h>
#include <string>

Setting_Screen::Setting_Screen() : Screen(true, true, true) {
	light_mode = Setting_Manager::light;
	theme = Setting_Manager::theme;
	sfx_volume = Setting_Manager::sfx_volume;
	music_volume = Setting_Manager::music_volume;
	font = Setting_Manager::font;

	setup_helper(  "Settings", 400.f, 10.f, 96.f, NULL);
	setup_helper(     "Theme", 20.f, 150.f, NULL, NULL);
	setup_helper("Light Mode", 20.f, 200.f, NULL, NULL);
	setup_helper(      "Font", 20.f, 250.f, NULL, NULL);
	setup_helper("SFX Volume", 20.f, 300.f, NULL, NULL);
	setup_helper(     "Music", 20.f, 350.f, NULL, NULL);

	for (int i = 0; i < themes; i++)
		setup_helper(std::to_string(i).c_str(), 300.f + (50 * i), 150.f, NULL, NULL);

	setup_helper( "Dark", 300.f, 200.f, NULL, NULL);
	setup_helper("Light", 400.f, 200.f, NULL, NULL);

	setup_helper( "Open Sans", 300.f, 250, NULL, NULL);
	setup_helper("Comic Neue", 500.f, 250.f, NULL, NULL);
	setup_helper("Montserrat", 700.f, 250.f, NULL, NULL);

	for (int i = 0; i <= 4; i++)
		setup_helper(std::to_string(i * 25).c_str(), 300.f + (50 * i), 300.f, NULL, NULL);
	for (int i = 0; i <= 4; i++)
		setup_helper(std::to_string(i * 25).c_str(), 300.f + (50 * i), 350.f, NULL, NULL);

	setup_helper(                          "Selected", 1000.f, 100.f, NULL, NULL);
	setup_helper(       std::to_string(theme).c_str(), 1000.f, 150.f, NULL, NULL);
	setup_helper(       light_mode ? "Light" : "Dark", 1000.f, 200.f, NULL, NULL);
	setup_helper(Font_Manager::get_selected().getInfo().family.c_str(), 1000.f, 250.f, NULL, NULL);
	setup_helper(  std::to_string(sfx_volume).c_str(), 1000.f, 300.f, NULL, NULL);
	setup_helper(std::to_string(music_volume).c_str(), 1000.f, 350.f, NULL, NULL);

	setup_helper("abcdefghijklmnopqrstuvwxyz, ABCDEFGHIJKLMNOPQRSTUVWXYZ\n!@#$%%^&*()_+-=", 25.f, 510.f,  NULL, NULL);
	setup_helper(                                                                     NULL, 20.f, 500.f, 1150.f, 95.f);

	texts[0].setThemeAndHover(false);
	texts[1].setThemeAndHover(false);
	texts[2].setThemeAndHover(false);
	texts[3].setThemeAndHover(false);
	texts[4].setThemeAndHover(false);
	texts[5].setThemeAndHover(false);
	texts[23].setThemeAndHover(false);
	texts[24].setThemeAndHover(false);
	texts[25].setThemeAndHover(false);
	texts[26].setThemeAndHover(false);
	texts[27].setThemeAndHover(false);
	texts[28].setThemeAndHover(false);
}

void Setting_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		light_mode = Setting_Manager::light;
		theme = Setting_Manager::theme;
		sfx_volume = Setting_Manager::sfx_volume;
		music_volume = Setting_Manager::music_volume;
		font = Setting_Manager::font;
		texts[24].setString(std::to_string(theme).c_str());
		texts[25].setString(light_mode ? "Light" : "Dark");
		texts[26].setString(Font_Manager::get_selected().getInfo().family.c_str());
		texts[27].setString(std::to_string(sfx_volume).c_str());
		texts[28].setString(std::to_string(music_volume).c_str());

		return_to_prev_screen(SettingScreen);
	}
	else if (mouse_in_helper(false, 6)) {
		theme = 0;
		texts[24].setString(std::to_string(theme).c_str());
	}
	else if (mouse_in_helper(false, 7)) {
		theme = 1;
		texts[24].setString(std::to_string(theme).c_str());
	}
	else if (mouse_in_helper(false, 8)) {
		light_mode = false;
		texts[25].setString("Dark");
	}
	else if (mouse_in_helper(false, 9)) {
		light_mode = true;
		texts[25].setString("Light");
	}
	else if (mouse_in_button(ConfirmButton) && (Setting_Manager::light != light_mode ||
		Setting_Manager::theme != theme || Setting_Manager::sfx_volume != sfx_volume ||
		Setting_Manager::music_volume != music_volume || Setting_Manager::font != font)) {
		Setting_Manager::light = light_mode;
		Setting_Manager::theme = theme;
		Setting_Manager::sfx_volume = sfx_volume;
		Setting_Manager::music_volume = music_volume;
		Setting_Manager::font = font;
		change_settings();
	}
	for (unsigned int i = 10; i < 13; i++)
		if (mouse_in_helper(false, i)) {
			font = i - 10;
			texts[26].setString(texts[i].getString());
		}
	for (unsigned int i = 13; i < 18; i++)
		if (mouse_in_helper(false, i)) {
			std::string string = texts[i].getString();
			sfx_volume = atoi(string.c_str());
			texts[27].setString(string);
		}
	for (unsigned int i = 18; i < 23; i++)
		if (mouse_in_helper(false, i)) {
			std::string string = texts[i].getString();
			music_volume = atoi(string.c_str());
			texts[28].setString(string);
		}
}

void Setting_Screen::hover_event_handler() {
	for (unsigned int i = 5; i < 23; i++) {
		bool in = mouse_in_helper(false, i);
		bool hover = texts[i].getHover();

		if (in && !hover)
			texts[i].highlight();
		else if (!in && hover)
			texts[i].highlight(false);
	}

	hover_textbox(0, 29);
}

void Setting_Screen::draw() {
	texts[28].setString(std::to_string(Setting_Manager::music_volume).c_str());
	Screen::draw();
}
