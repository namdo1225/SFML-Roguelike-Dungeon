/**
*
* File: interface.cpp
* Description: Contains the implementation of the Interface class.
*
*/

#include "interface.h"

#include "Screen/custom_screen.h"
#include "Screen/exit_screen.h"
#include "Screen/game_screen.h"
#include "Screen/inventory_screen.h"
#include "Screen/level_screen.h"
#include "Screen/load_screen.h"
#include "Screen/log_screen.h"
#include "Screen/map_screen.h"
#include "Screen/menu_screen.h"
#include "Screen/name_screen.h"
#include "Screen/setting_screen.h"
#include "Screen/shop_screen.h"
#include "Screen/spell_attack_screen.h"
#include "Screen/spell_screen.h"
#include "Screen/stat_screen.h"
#include "Screen/status_screen.h"
#include "Screen/title_screen.h"

#include "Manager/setting_manager.h"
#include <Floor/enemy.h>
#include <Manager/audio_manager.h>
#include <Manager/game_manager.h>
#include <Manager/texture_manager.h>
#include <memory>
#include <player.h>
#include <Screen/message_screen.h>
#include <Screen/screen.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>

Interface* Interface::singleton;

Interface::Interface() {
    if (!Spell::setup() || !Item::setup() || !Special::setup() ||
        !Texture_Manager::load() || !Audio_Manager::load() || !Enemy::setup() ||
        !Setting_Manager::load())
        return;
    Screen::setup();
    Game_Manager::setup();

    Audio_Manager::setMusicVolume(Setting_Manager::musicVolume);
    Audio_Manager::setSFXVolume(Setting_Manager::sfxVolume);

    Screen::screens[TitleScreen] =  std::make_unique<Title_Screen>();
    Screen::screens[ExitScreen] = std::make_unique<Exit_Screen>();
    Screen::screens[SettingScreen] = std::make_unique<Setting_Screen>();
    Screen::screens[NameScreen] = std::make_unique<Name_Screen>();
    Screen::screens[StatScreen] = std::make_unique<Stat_Screen>();
    Screen::screens[GameScreen] = std::make_unique<Game_Screen>();
    Screen::screens[MenuScreen] = std::make_unique<Menu_Screen>();
    Screen::screens[InventoryScreen] = std::make_unique<Inventory_Screen>();
    Screen::screens[SpellScreen] = std::make_unique<Spell_Screen>();
    Screen::screens[LevelScreen] = std::make_unique<Level_Screen>();
    Screen::screens[LoadScreen] = std::make_unique<Load_Screen>();
    Screen::screens[MapScreen] = std::make_unique<Map_Screen>();
    Screen::screens[LogScreen] = std::make_unique<Log_Screen>();
    Screen::screens[ShopScreen] = std::make_unique<Shop_Screen>();
    Screen::screens[SpellAttackScreen] = std::make_unique<Spell_Attack_Screen>();
    Screen::screens[StatusScreen] = std::make_unique<Status_Screen>();
    Screen::screens[CustomScreen] = std::make_unique<Custom_Screen>();
    Screen::screens[MessageScreen] = std::make_unique<Message_Screen>();

    Screen::changeSettings();
    Game_Manager::player = Player();
    Game_Manager::resetGame();
}

Interface::~Interface() {}

Interface& Interface::get() {
    static Interface singleton;
    return singleton;
}

void Interface::loopWindow() {
    while (window.isOpen())
    {
        handleEvent();
        Game_Manager::delEnemy();

        window.clear();
        draw();
        window.display();
    }
}
void Interface::draw() {
    for (unsigned int i = 0; i < ExitScreen + 1; i++)
        if (Screen::visibilities[i]) {
            if (Screen::screens[i]->getUpdate())
                Screen::screens[i]->updateDraw();
            Screen::screens[i]->draw();
        }
}
void Interface::handleEvent() {
    sf::Event& event = Screen::event;

    while (window.pollEvent(event)) {
        Game_Manager::checkPlayerPath();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        window.setView(Game_Manager::viewWorld);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos);
        window.setView(Game_Manager::viewUI);

        sf::Vector2f mouseUI = window.mapPixelToCoords(mousePos);
        Screen::x = mouseUI.x;
        Screen::y = mouseUI.y;

        Screen::worldX = mouseWorld.x;
        Screen::worldY = mouseWorld.y;

        window.setView(Game_Manager::viewSlots);
        sf::Vector2f mouseSlots = window.mapPixelToCoords(mousePos);
        Screen::slotX = mouseSlots.x;
        Screen::slotY = mouseSlots.y;
        window.setView(Game_Manager::viewUI);

        if ((event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) &&
            Screen::display != ExitScreen) {
            Screen::openDialog(Screen::display, ExitScreen);
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            Screen::click() || Screen::screens[Screen::display]->handleClickEvent();
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased ||
            event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseWheelScrolled)
            Screen::screens[Screen::display]->handleMouseEvent();

        Screen::screens[Screen::display]->handleHoverEvent();
        Screen::screens[Screen::display]->hover();

        if (event.type == sf::Event::TextEntered)
            Screen::screens[Screen::display]->handleTextEvent();

        if (event.type == sf::Event::KeyPressed)
            Screen::screens[Screen::display]->handleKeyEvent();
    }
}