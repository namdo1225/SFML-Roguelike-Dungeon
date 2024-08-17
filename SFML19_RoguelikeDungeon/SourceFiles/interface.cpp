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

Interface* Interface::singleton;

Interface::Interface() {
    if (!Texture_Manager::load() || !Audio_Manager::load())
        return;
    Screen::setup();
    Game_Manager::setup();
    Setting_Manager::load();
    Audio_Manager::set_music_volume(Setting_Manager::music_volume);
    Audio_Manager::set_sfx_volume(Setting_Manager::sfx_volume);

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

    Screen::change_settings();
    Game_Manager::player = Player();
    Game_Manager::reset_game();
}

Interface::~Interface() {}

Interface& Interface::get() {
    static Interface singleton;
    return singleton;
}

void Interface::window_loop() {
    while (window.isOpen())
    {
        handle_event();
        Game_Manager::ene_dead();

        window.clear();
        draw_interface();
        window.display();
    }
}
void Interface::draw_interface() {
    for (unsigned int i = 0; i < ExitScreen + 1; i++)
        if (Screen::visibilities[i]) {
            if (Screen::screens[i]->get_update())
                Screen::screens[i]->update_draw();
            Screen::screens[i]->draw();
        }
}
void Interface::handle_event() {
    sf::Event& event = Screen::event;



    while (window.pollEvent(event)) {
        Game_Manager::pl_move_obstacle();
        Screen::mouse_pos = sf::Mouse::getPosition(window);
        Screen::mouse_world = window.mapPixelToCoords(Screen::mouse_pos);
        Screen::x = Screen::mouse_world.x;
        Screen::y = Screen::mouse_world.y;
        sf::Vector2f ui = Screen::viewUI.getCenter();
        sf::Vector2f world = Screen::viewWorld.getCenter();
        float dist_x = ui.x - world.x;
        float dist_y = ui.y - world.y;
        Screen::world_x = Screen::x - dist_x;
        Screen::world_y = Screen::y - dist_y;

        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            Screen::show_dialog(Screen::display, ExitScreen);
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            Screen::screens[Screen::display]->click_event_handler();
            Screen::click();
        } 
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased ||
            event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseWheelScrolled)
            Screen::screens[Screen::display]->mouse_event_handler();

        Screen::screens[Screen::display]->hover_event_handler();
        Screen::screens[Screen::display]->hover();

        if (event.type == sf::Event::TextEntered)
            Screen::screens[Screen::display]->handleTextEvent();

        if (event.type == sf::Event::KeyPressed)
            Screen::screens[Screen::display]->key_event_handler();
    }
}