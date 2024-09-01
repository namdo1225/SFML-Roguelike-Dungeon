/**
*
* File: font_manager.cpp
* Description: Contains the implementation of the Font_Manager class.
*
*/

#include "Manager/font_manager.h"
#include <filesystem>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <vector>

std::vector<sf::Font> Font_Manager::buffers;
bool Font_Manager::assets_loaded = false;
unsigned int Font_Manager::selected_font = 0;

Font_Manager::Font_Manager() {
    if (!assets_loaded)
        load();
}

bool Font_Manager::load() {
    if (assets_loaded)
        return true;

    for (auto& file : std::filesystem::directory_iterator("Font")) {
        const std::string path = file.path().string();

        buffers.push_back(sf::Font());

        if (!buffers[buffers.size() - 1].loadFromFile(path))
            return false;
    }

    assets_loaded = !buffers.empty();
    return assets_loaded;
}

sf::Font& Font_Manager::get(unsigned int i) {
    if (!assets_loaded)
        load();

    return (i < 0 && i >= buffers.size()) ? buffers[selected_font] : buffers[i];
}

void Font_Manager::set(unsigned int i) {
    if (i >= 0 && i < buffers.size())
        selected_font = i;
}

sf::Font& Font_Manager::get_selected() {
    if (!assets_loaded)
        load();

    return buffers[selected_font];
}

unsigned int Font_Manager::getNumFonts() {
    return buffers.size();
}

std::string Font_Manager::getFontName(unsigned int i) {
    return (i < 0 && i >= buffers.size()) ? buffers[selected_font].getInfo().family : buffers[i].getInfo().family;
}