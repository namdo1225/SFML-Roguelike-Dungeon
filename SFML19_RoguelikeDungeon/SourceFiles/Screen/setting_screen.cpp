/**
*
* File: setting_screen.cpp
* Description: Contain the implementation of the Setting_Screen class.
*/


#include "Screen/setting_screen.h"
#include <cstdio>
#include <malloc.h>
#include <Manager/database_manager.h>
#include <Manager/font_manager.h>
#include <Manager/setting_manager.h>
#include <nfd.h>
#include <Screen/screen.h>
#include <Shape/full_textinput.h>
#include <string>

bool Setting_Screen::light_mode = false;
unsigned int Setting_Screen::theme = 0;
unsigned int Setting_Screen::font = 0;
std::string Setting_Screen::saveLocation;

Setting_Screen::Setting_Screen() : Screen(true, true, true) {
	update = true;

	light_mode = Setting_Manager::light;
	theme = Setting_Manager::theme;
	font = Setting_Manager::font;
	saveLocation = Setting_Manager::saveLocation;

	textRectH(  "Settings", 400.f, 10.f, 96.f, NULL, false);
	textRectH(     "Theme", 20.f, 150.f, NULL, NULL, false);
	textRectH("Light Mode", 20.f, 200.f, NULL, NULL, false);
	textRectH(      "Font", 20.f, 250.f, NULL, NULL, false);
	textRectH("SFX Volume", 20.f, 300.f, NULL, NULL, false);
	textRectH(     "Music", 20.f, 350.f, NULL, NULL, false);
	textRectH("Save Location", 20.f, 400.f, NULL, NULL, false);
	textRectH("Selected", 1000.f, 100.f, NULL, NULL, false);

	for (int i = 0; i < 11; i++) {
		hoverableTextH(std::to_string(i * 10).c_str(), 300.f + (50 * i), 300.f, [this, i]() {
			textInputs[1].text.setString(std::to_string(i * 10));
		});
	}

	for (int i = 0; i < 11; i++) {
		unsigned int j = i + 10;
		hoverableTextH(std::to_string(i * 10).c_str(), 300.f + (50 * i), 350.f, [this, i]() {
			textInputs[2].text.setString(std::to_string(i * 10));
		});
	}

	for (int i = 0; i < 2; i++) {
		unsigned int j = i + 20;
		hoverableTextH(i ? "Dark" : "Light", 300.f + (100.f * i), 200.f, [this, i]() {
			texts[9].setString(i ? "Dark" : "Light");
		});
	}

	for (int i = 0; i < THEMES; i++) {
		unsigned int j = i + 22;
		hoverableTextH(std::to_string(i).c_str(), 300.f + (100.f * i), 150.f, [this, i]() {
			textInputs[0].text.setString(std::to_string(i));
		});
	}

	for (int i = 0; i < Font_Manager::getNumFonts(); i++) {
		unsigned int j = i + 22 + THEMES;
		hoverableTextH(Font_Manager::getFontName(i).c_str(), 300.f + (100 * i), 250.f, [this, i]() {
			font = i;
			texts[10].setString(Font_Manager::getFontName(font));
		}, 12.f);
	}

	// starting with index 8
	textRectH(       light_mode ? "Light" : "Dark", 1000.f, 200.f, NULL, NULL, false);
	textRectH(Font_Manager::getSelected().getInfo().family.c_str(), 1000.f, 250.f, NULL, NULL, false);
	//textRectH(  std::to_string(sfxVolume).c_str(), 1000.f, 300.f, NULL, NULL, false);
	//textRectH(std::to_string(musicVolume).c_str(), 1000.f, 350.f, NULL, NULL, false);
	textRectH(saveLocation.c_str(), 300.f, 400.f, 14.f, NULL, false);

	textInputH(std::to_string(theme).c_str(), 3, 1000.f, 140.f, 70.f, 40.f, NumberValidation, 0.0f, 0.0f, 0, 1);
	textInputH(std::to_string(sfxVolume).c_str(), 3, 1000.f, 290.f, 70.f, 40.f, NumberValidation, 0.0f, 0.0f, 0, 100);
	textInputH(std::to_string(musicVolume).c_str(), 3, 1000.f, 340.f, 70.f, 40.f, NumberValidation, 0.0f, 0.0f, 0, 100);


	textboxH("abcdefghijklmnopqrstuvwxyz, ABCDEFGHIJKLMNOPQRSTUVWXYZ\n!@#$%%^&*()_+-=", 20.f, 500.f, 1150.f, 95.f, NULL);

	textboxH("Reset Database", 850.f, 700.f, 300.f, 50.f, []() {
		Database_Manager::resetDB();
		window.close();
	});

	textboxH("Change Save Location", 50.f, 700.f, 300.f, 50.f, [this]() {
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("sav", NULL, &outPath);

		if (result == NFD_CANCEL) {
			printf("Error: User pressed cancel.");
			return;
		}
		else if (result != NFD_OKAY) {
			printf("Error: %s\n", NFD_GetError());
			return;
		}

		saveLocation = outPath;
		texts[10].setString(saveLocation);

		free(outPath);
	});

	textboxH("Clear Save Location", 50.f, 630.f, 300.f, 50.f, [this]() {
		saveLocation = "";
		texts[10].setString("");
	});
}

bool Setting_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		light_mode = Setting_Manager::light;
		theme = Setting_Manager::theme;
		sfxVolume = Setting_Manager::sfxVolume;
		musicVolume = Setting_Manager::musicVolume;
		font = Setting_Manager::font;
		saveLocation = Setting_Manager::saveLocation;
		texts[8].setString(light_mode ? "Light" : "Dark");
		texts[9].setString(Font_Manager::getSelected().getInfo().family);
		texts[10].setString(saveLocation);
		textInputs[0].text.setString(std::to_string(theme));
		textInputs[1].text.setString(std::to_string(sfxVolume));
		textInputs[2].text.setString(std::to_string(musicVolume));

		goToPrevScreen(SettingScreen);
		return true;
	}
	else if (mouseInButton(ConfirmButton)) {
		int theme = std::stoi(textInputs[0].text.getString().toAnsiString());
		int sfxVolume = std::stoi(textInputs[1].text.getString().toAnsiString());
		int musicVolume = std::stoi(textInputs[2].text.getString().toAnsiString());

		if (Setting_Manager::light != light_mode ||
			Setting_Manager::theme != theme || Setting_Manager::sfxVolume != sfxVolume ||
			Setting_Manager::musicVolume != musicVolume || Setting_Manager::font != font ||
			Setting_Manager::saveLocation != saveLocation) {
			Setting_Manager::light = light_mode;
			Setting_Manager::theme = theme;
			Setting_Manager::sfxVolume = sfxVolume;
			Setting_Manager::musicVolume = musicVolume;
			Setting_Manager::font = font;
			Setting_Manager::saveLocation = saveLocation;
			changeSettings();
		}
	}
}

void Setting_Screen::updateDraw() {
	texts[12].setString(std::to_string(Setting_Manager::musicVolume));
	update = false;
}
