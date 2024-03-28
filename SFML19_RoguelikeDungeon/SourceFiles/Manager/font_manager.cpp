/**
*
* File: font_manager.cpp
* Description: Contains the implementation of the Font_Manager class.
*
*/

#include "Manager/font_manager.h"
#include <iostream>

sf::Font Font_Manager::buffers[Font_Manager::fonts] = { sf::Font() };
bool Font_Manager::assets_loaded = false;
const unsigned int Font_Manager::fonts;
unsigned int Font_Manager::selected_font = 0;

Font_Manager::Font_Manager() {
    if (!assets_loaded)
        load();
}

bool Font_Manager::load() {
    if (assets_loaded)
        return true;

    if (!buffers[0].loadFromFile("Font\\OpenSans.ttf"))
        return false;
    if (!buffers[1].loadFromFile("Font\\ComicNeue.ttf"))
        return false;
    if (!buffers[2].loadFromFile("Font\\Montserrat.ttf"))
        return false;

    assets_loaded = true;
    return assets_loaded;
}

sf::Font& Font_Manager::get(unsigned int i) {
    if (!assets_loaded)
        load();

    return (i < 0 && i >= fonts) ? buffers[selected_font] : buffers[i];
}

void Font_Manager::set(unsigned int i) {
    if (i >= 0 && i < fonts)
        selected_font = i;
}

sf::Font& Font_Manager::get_selected() {
    if (!assets_loaded)
        load();

    return buffers[selected_font];
}