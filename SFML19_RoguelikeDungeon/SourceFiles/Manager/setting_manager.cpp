/**
*
* File: settings_manager.cpp
* Description: Contains the implementation of the Setting_Manager class.
*
*/

#include "Manager/setting_manager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <format>

using json = nlohmann::json;
unsigned int Setting_Manager::theme = 0;
unsigned int Setting_Manager::music_volume = 100;
unsigned int Setting_Manager::sfx_volume = 100;
bool Setting_Manager::light = false;
unsigned int Setting_Manager::font = 0;

const unsigned int Setting_Manager::themes;

Setting_Manager::Setting_Manager() {
}

bool Setting_Manager::load()
{
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::badbit);
        file.open("settings.json");
        if (!file) {
            save();
            return false;
        }

        json j = json::parse(file);
        theme = j.at("theme");
        light = j.at("light");
        sfx_volume = j.at("sfxVolume");
        music_volume = j.at("musicVolume");
        font = j.at("font");
    }
    catch (const std::exception&) {
        save();
        return false;
    }
    return true;;
}

bool Setting_Manager::save(bool create)
{
    json j;
    try {
        j["theme"] = create ? 0 : theme;
        j["light"] = create ? false : light;
        j["sfxVolume"] = create ? 100 : sfx_volume;
        j["musicVolume"] = create ? 100 : music_volume;
        j["font"] = create ? 0 : font;
    }
    catch (const std::exception&) {
        return false;
    };

    try {
        std::ofstream file_out{ "settings.json" };
        if (!file_out) {
            return false;
        }

        file_out << std::setw(4) << j << std::endl;
        file_out.close();
    }
    catch (const std::exception&) {
        return false;
    }

    return true;
}