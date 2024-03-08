/**
*
* File: interface.cpp
* Description: Contains the implementation of the Interface class.
*
*/

#include "interface.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <json.hpp>
#include <format>

using json = nlohmann::json;

Interface* Interface::singleton;

Interface::Interface() {
    if (!font.loadFromFile("Font\\open-sans-latin-400-normal.ttf"))
        return;

    window.create(sf::VideoMode(1200, 800), "RE: Dungeon", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    if (!Texture_Manager::load() || !Audio_Manager::load())
        return;

    background.setPosition(-10, -10);
    background.setSize(sf::Vector2f(1210, 810));
    background.setFillColor(sf::Color::Black);
    background.setOutlineThickness(3.f);

    create_title_ui();
    create_name_ui();
    create_exit_ui();
    create_stat_ui();
    create_main_ui();
    create_lvl_ui();
    create_ext_ui();
    create_inv_ui();
    create_sp_ui();
    create_shop_ui();
    create_ld_ui();
    Map::setup(font);
    player = Player(font);

    reset_game();
}

Interface::~Interface() {
    select_itm = new Place_Holder();
    inv_draw_desc = new Place_Holder();
    inv_select = new Place_Holder();

    select_sp = new PH_Spell();
    spell_desc = new PH_Spell();
    spell_select = new PH_Spell();

    delete select_itm, delete select_sp, delete pl_weapon, delete pl_armor, delete inv_draw_desc,
        delete inv_select, delete spell_desc, delete spell_select;

    items.clear();
    spells.clear();

    delete sh_buy_sp[0], delete sh_buy_sp[1], delete sh_buy_sp[2];

    delete sh_buy_it[0], delete sh_buy_it[1], delete sh_buy_it[2], delete sh_buy_it[3], delete sh_buy_it[4], delete sh_buy_it[5];
}

Interface& Interface::get() {
    static Interface singleton;
    return singleton;
}

// major functionalities
void Interface::next_level() {
    if (player.get_pos('x') != floor.get_stair_pos('x') || player.get_pos('y') != floor.get_stair_pos('y'))
        return;

    viewUI.reset(sf::FloatRect(0, 0, 1200, 800));
    viewWorld.reset(sf::FloatRect(0, 0, 1200, 800));

    Audio_Manager::play_sfx(1);
    player.set_floor(player.get_floor() + 1);

    main_current_floor_txt.setString("Floor: " + std::to_string(player.get_floor()));

    floor = Floor();
    pl_random_pos();
    enemies.clear();
    ene_add();
    floor.make_collectible(player.get_floor());
    floor.make_gold(player.get_floor());
    floor.make_interactible(player.get_floor());

    center_floor();
    enemy_respawn = 5;
    floor_copied = false;
}
void Interface::reset_game() {
    viewUI.reset(sf::FloatRect(0, 0, 1200, 800));
    viewWorld.reset(sf::FloatRect(0, 0, 1200, 800));

    title = stat_screen = true;
    exit_menu = name_screen = lvl_screen = main_screen = ext_screen = 
        inv_screen = sp_screen = floor_copied = false;
    
    name_ph_1 = "Player";
    name_ph_2.setString("Player");
    player.reset();

    main_level_text[1].setString(std::to_string(player.get_lvl()));
    main_level_text[2].setString(std::to_string(player.get_cur_exp()) + " / " + std::to_string(player.get_lvl_up()));
    main_current_floor_txt.setString("Floor: " + std::to_string(player.get_floor()));
    main_gold_txt.setString("Gold: " + std::to_string(player.get_gold()));

    floor = Floor();
    pl_random_pos();
    enemies.clear();
    ene_add();
    enemy_respawn = rand() % 5 + 1;
    floor.make_collectible(player.get_floor());
    floor.make_gold(player.get_floor());
    floor.make_interactible(player.get_floor());
    center_floor();

    items.clear();
    itm_add_item(new Iron_Sword(font));
    itm_add_item(new Iron_Armor(font));
    itm_add_item(new Health_Potion(font));
    itm_add_item(new Mana_Potion(font));
    
    spells.clear();
    sp_add_spell(new Fire(font));
    sp_add_spell(new Heal(font));


    itm_equip_weapon(items[0]);
    itm_equip_armor(items[1]);
    itm_reoganize_inv();
    itm_select_shortcut('n');

    main_log.clear();

    ld_savename_str = "Player";
    ld_savename_txt.setString("Player");
}
void Interface::center_floor() {
    int x{ player.get_pos('x') }, y{ player.get_pos('y') };

    off_x = (x > 400) ? -(x - 400) : 400 - x;
    off_y = (y > 400) ? -(y - 400) : 400 - y;

    player.set_pos(x + off_x, y + off_y);

    for (Room& rm : floor.rooms) {
        rm.set_pos_and_size(rm.get_rm('x') + off_x, rm.get_rm('y') + off_y, -1, -1);
        if (rm.door_exist())
            rm.set_door(rm.get_door('x') + off_x, rm.get_door('y') + off_y, -1);
    }

    for (Enemy& en : enemies)
        en.set_pos(en.get_pos('x') + off_x, en.get_pos('y') + off_y);

    for (Collectible& col : floor.collectibles)
        col.set_pos(col.get_pos('x') + off_x, col.get_pos('y') + off_y);

    for (Gold_Collectible& gold : floor.golds)
        gold.set_pos(gold.get_pos('x') + off_x, gold.get_pos('y') + off_y);

    for (Interactible& interactible : floor.interactibles)
        interactible.set_pos(interactible.get_pos('x') + off_x, interactible.get_pos('y') + off_y);

    floor.set_stair_pos(floor.get_stair_pos('x') + off_x, floor.get_stair_pos('y') + off_y);
    floor.set_shop_pos(floor.get_shop_pos('x') + off_x, floor.get_shop_pos('y') + off_y);
}
void Interface::window_loop() {
    while (window.isOpen())
    {
        sf::Event event;
        handle_event(event);

        pl_game_over();
        ene_dead();
        window.clear();
        draw_interface();
        window.display();
    }
}
void Interface::draw_interface() {
    window.draw(background);

    if (exit_menu) draw_exit_screen();
    else if (title) draw_title_screen();
    else if (name_screen && !exit_menu) {
        draw_name_screen();
        if (player.get_name() != "")
            name_screen = false;
    }
    else if (stat_screen) draw_stat_selection();
    else if (lvl_screen) draw_lvl_ui();
    else if (main_screen) draw_main_ui();
    else if (inv_screen) draw_inv_ui();
    else if (sp_screen) draw_sp_ui();
    else if (map_menu) draw_map_ui();
    else {
        draw_main_ui();
        draw_ext_ui();
    }
}
void Interface::handle_event(sf::Event& event) {
    while (window.pollEvent(event)) {
        pl_move_obstacle();
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };

        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            exit_menu = true;
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            handle_exit_prompt(x, y);
            handle_title_prompt(x, y);
            handle_stat_pick_prompt(x, y);
            handle_lvl_prompt(x, y);
            handle_main_prompt(x, y);
            handle_ext_prompt(x, y);
            handle_inv_prompt(x, y);
            handle_spell_prompt(x, y);
            handle_map_prompt(x, y);
        }

        if (main_screen && !exit_menu) {
            if (event.type == sf::Event::KeyPressed) {
                if (!player.is_stuck(0) && event.key.code == sf::Keyboard::Up)
                    handle_player_action('u', 0);
                else if (!player.is_stuck(1) && event.key.code ==  sf::Keyboard::Right)
                    handle_player_action('r', 0);
                else if (!player.is_stuck(2) && event.key.code == sf::Keyboard::Down)
                    handle_player_action('d', 0);
                else if (!player.is_stuck(3) && event.key.code == sf::Keyboard::Left)
                    handle_player_action('l', 0);
            }

            if (event.mouseButton.button == sf::Mouse::Left) {
                if (x >= 400 && x <= 440 && y >= 360 - ((pl_weapon->get_range() - 1) * 40) && y <= 400)
                    handle_player_action('u', 1);
                else if (x >= 440 && x <= 480 + ((pl_weapon->get_range() - 1) * 40) && y >= 400 && y <= 440)
                    handle_player_action('r', 1);
                else if (x >= 400 && x <= 440 && y >= 440 && y <= 480 + ((pl_weapon->get_range() - 1) * 40))
                    handle_player_action('d', 1);
                else if (x >= 360 - ((pl_weapon->get_range() - 1) * 40) && x <= 400 && y >= 400 && y <= 440)
                    handle_player_action('l', 1);
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
                ene_overall();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                next_level();
                handle_shop();
            }
        }

        if (!exit_menu && name_screen)
            name_creation(event);
    }
}

// handle save events
void Interface::save() {
    json j;
    try {
        j["name"] = player.get_name();
        j["floorNumber"] = player.get_floor();
        j["level"] = player.get_floor();
        j["gold"] = player.get_gold();
        j["statPoints"] = player.get_pts();

        j["player"] = {
            {"x", player.get_pos('x')},
            {"y", player.get_pos('y')},
        };

        j["exp"] = {
            {"current", player.get_cur_exp() },
            {"levelUp", player.get_lvl_up() }
        };
        j["inventorySlots"] = player.get_max_itm();

        j["stat"] = {
            {"hp",
                {
                    {"current", player.get_stat(6)},
                    {"max", player.get_stat(0)}
                }
            },
            {"mp",
                {
                    {"current", player.get_stat(7)},
                    {"max", player.get_stat(1)}
                }
            },
            {"str", player.get_stat(2)},
            {"mgk", player.get_stat(3)},
            {"def", player.get_stat(4)},
            {"res", player.get_stat(5)},
        };

        if (!items.empty())
            for (Item* itm : items)
                j["inventory"].push_back(itm->get_id());

        j["weaponSlot"] = std::find(items.begin(), items.end(), pl_weapon) - items.begin();
        j["armorSlot"] = std::find(items.begin(), items.end(), pl_armor) - items.begin();

        if (!spells.empty())
            for (Spell* sp : spells)
                j["spell"].push_back(sp->get_id());

        for (unsigned int i{ 0 }; i < floor.rooms.size(); i++) {
            j["floor"].push_back(json::object());
            j["floor"][i] = {
                {"x", floor.rooms[i].get_rm('x')},
                {"y", floor.rooms[i].get_rm('y')},
                {"sizeX", floor.rooms[i].get_rm('w')},
                {"sizeY", floor.rooms[i].get_rm('h')},
            };
            if (floor.rooms[i].door_exist()) {
                j["floor"][i]["door"] = {
                    {"x", floor.rooms[i].get_door('x')},
                    {"y", floor.rooms[i].get_door('y')},
                    {"rotation", floor.rooms[i].get_door('r')},
                    {"door0", floor.rooms[i].get_door('0')},
                    {"door1", floor.rooms[i].get_door('1')},
                    {"door2", floor.rooms[i].get_door('2')},
                    {"door3", floor.rooms[i].get_door('3')},
                };
            }
        }

        j["stair"] = {
            {"x", floor.get_stair_pos('x')},
            {"y", floor.get_stair_pos('y')},
        };

        if (floor.shop_exist()) {
            j["shop"] = {
                {"x", floor.get_shop_pos('x')},
                {"y", floor.get_shop_pos('y')},
            };
        }

        j["enemiesRespawn"] = enemy_respawn;
        for (Enemy en : enemies) {
            j["enemies"].push_back(json::object());
            unsigned int i = j["enemies"].size() - 1;
            j["enemies"][i] = {
                {"id", en.get_id()},
                {"health", en.get_stat(0)},
                {"x", en.get_pos('x')},
                {"y", en.get_pos('y')},
            };
        }

        for (Collectible col : floor.collectibles) {
            j["items"].push_back(json::object());
            unsigned int i = j["items"].size() - 1;
            j["items"][i] = {
                {"id", col.get_id()},
                {"x", col.get_pos('x')},
                {"y", col.get_pos('y')},
            };
        }

        for (Gold_Collectible gold : floor.golds) {
            j["golds"].push_back(json::object());
            unsigned int i = j["golds"].size() - 1;
            j["golds"][i] = {
                {"amount", gold.get_amount()},
                {"x", gold.get_pos('x')},
                {"y", gold.get_pos('y')},
            };
        }

        for (Interactible interactible : floor.interactibles) {
            j["interactibles"].push_back(json::object());
            unsigned int i = j["interactibles"].size() - 1;
            j["interactibles"][i] = {
                {"x", interactible.get_pos('x')},
                {"y", interactible.get_pos('y')},
            };
        }

        for (const Effect& effect : player.get_effects()) {
            j["effects"].push_back(json::object());
            unsigned int i = j["effects"].size() - 1;
            j["effects"][i] = {
                {"stat", effect.stat_changed},
                {"difference", effect.stat_difference},
                {"turn", effect.change_turns},
                {"original", effect.original_turns},
            };
        }
    }
    catch (const std::exception&) {
        log_add("An error occurred while gathering data to save.");
    };

    try {
        std::ofstream file_out{ std::format("{}.sav", player.get_name()) };
        if (!file_out) {
            log_add("Save not successful.");
            return;
        }

        file_out << std::setw(4) << j << std::endl;
        file_out.close();
        log_add("Saved successfully.");
    }
    catch (const std::exception&) {
        log_add("Save not successful.");
    }
}
void Interface::load() {
    std::string file_name;

    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60) {
                    ld_savename_str = "Player";
                    ld_savename_txt.setString("Player");
                    return;
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    file_name = ld_savename_str;
                    read_save(file_name + ".sav");
                    return;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                    if (ld_savename_str.size() != 0)
                        ld_savename_str.pop_back();

                    (ld_savename_str.size() == 0) ? ld_savename_txt.setString("") : ld_savename_txt.setString(ld_savename_str);
                }
                else if (ld_savename_str.size() == 20)
                    continue;
                else if (event.text.unicode < 128) {
                    char letter{ static_cast<char>(event.text.unicode) };

                    if (letter == '\\' || letter == ' ' || letter == '=' || letter == '$' || letter == '*' or
                        letter == '|' || letter == '~' || letter == '.' || letter == '"' || letter == '\'')
                        continue;

                    ld_savename_str += static_cast<char>(event.text.unicode);
                    ld_savename_txt.setString(ld_savename_str);
                }
            }
        }

        window.clear();
        window.draw(background);

        window.draw(ld_direction_txt);
        window.draw(ld_savename_txt);

        window.draw(level_up_exit_rect);
        window.draw(level_up_exit_txt);

        window.draw(name_line);

        window.display();
    }
}
void Interface::read_save(std::string file_name) {
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::badbit);
        file.open(file_name.c_str());
        if (!file) {
            title = true;
            return;
        }

        json j = json::parse(file);
        player.set_name(j.at("name"));

        player.set_floor(j.at("floorNumber"));
        main_current_floor_txt.setString("Floor: " + std::to_string(player.get_floor()));

        player.set_pos(j.at("player").at("x"), j.at("player").at("y"));

        player.set_level(j.at("level"));
        player.set_gold(j.at("gold"));
        player.set_point(j.at("statPoints"));
        player.set_cur_exp(j.at("exp").at("current"));
        player.set_lvl_up(j.at("exp").at("levelUp"));
        player.set_max_item(j.at("inventorySlots"));

        player.set_stat(6, j.at("stat").at("hp").at("current"));
        player.set_stat(0, j.at("stat").at("hp").at("max"));

        player.set_stat(7, j.at("stat").at("mp").at("current"));
        player.set_stat(1, j.at("stat").at("mp").at("max"));

        player.set_stat(2, j.at("stat").at("str"));
        player.set_stat(3, j.at("stat").at("mgk"));
        player.set_stat(4, j.at("stat").at("def"));
        player.set_stat(5, j.at("stat").at("res"));

        items.clear();
        for (auto& element : j.at("inventory")) {
            itm_add_item(Item::create_itm(element, font));
        }
        itm_equip_weapon(items[j.at("weaponSlot")]);
        itm_equip_armor(items[j.at("armorSlot")]);

        spells.clear();
        for (auto& element : j.at("spell")) {
            sp_add_spell(Spell::create_spell(element, font));
        }

        floor = Floor(true);
        for (auto& room : j.at("floor")) {
            floor.load_room(room.at("x"), room.at("y"), room.at("sizeX"), room.at("sizeY"));
            if (room.contains("door")) {
                auto& door = room.at("door");
                floor.load_door(door.at("x"), door.at("y"), door.at("rotation"), door.at("door0"), door.at("door1"), door.at("door2"), door.at("door3"));
            }
        }
        floor.make_map();

        floor.load_stair(j.at("stair").at("x"), j.at("stair").at("y"));
        if (j.contains("shop"))
            floor.load_stair(j.at("shop").at("x"), j.at("shop").at("y"));
        
        enemy_respawn = j.at("enemiesRespawn");
        enemies.clear();
        if (j.contains("enemies"))
            for (auto& enemy : j.at("enemies"))
                enemies.push_back(Enemy(enemy.at("health"), player.get_floor(), enemy.at("id"), enemy.at("x"), enemy.at("y")));

        if (j.contains("items"))
            for (auto& item : j.at("items"))
                floor.load_collectible(item.at("x"), item.at("y"), item.at("id"));

        if (j.contains("golds"))
            for (auto& gold : j.at("golds"))
                floor.load_gold(gold.at("x"), gold.at("y"), gold.at("amount"));

        if (j.contains("interactibles"))
            for (auto& interactible : j.at("interactibles"))
                floor.load_interactible(interactible.at("x"), interactible.at("y"));

        player.reset_effect();

        if (j.contains("effects"))
            for (auto& effect : j.at("effects"))
                player.set_effect(effect.at("stat"), effect.at("difference"), effect.at("original"), effect.at("turn"));

        stat_screen = title = false;
        main_screen = true;
        main_log.clear();
        file.close();
        center_floor();

        log_add("The maze changed its appearance...");
        log_add("Save loaded.");
    }
    catch (const std::exception&) {
        printf("Error while trying to open save file.");
    }

    ld_savename_str = "Player";
    ld_savename_txt.setString("Player");
}

// handle player events
void Interface::pl_random_pos() {
    int i{ rand() % static_cast<int>(floor.rooms.size()) };
    int temp_x = ((rand() % (floor.rooms[i].get_rm('w') / 40)) + (floor.rooms[i].get_rm('x') / 40)) * 40,
    temp_y = ((rand() % (floor.rooms[i].get_rm('h') / 40)) + (floor.rooms[i].get_rm('y') / 40)) * 40;

    player.set_pos(temp_x, temp_y);
}
void Interface::pl_move_obstacle() {
    int x{ player.get_pos('x') }, y{ player.get_pos('y') }, x2{ x + 40 }, y2{ y + 40 };

    // Check which room player is in && determines if they're touching a wall.
    for (Room rm : floor.rooms)
        if (rm.in_room(x, y, x2, y2)) {
            (y == rm.get_rm('y')) ? player.set_stuck(0, 1) : player.set_stuck(0, 0);
            (x2 == rm.get_rm('1')) ? player.set_stuck(1, 1) : player.set_stuck(1, 0);
            (y2 == rm.get_rm('2')) ? player.set_stuck(2, 1) : player.set_stuck(2, 0);
            (x == rm.get_rm('x')) ? player.set_stuck(3, 1) : player.set_stuck(3, 0);
            break;
        }

    // Check rooms that have doors && whether player is touching a door. It then allows player the ability to move through it.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].touch_door(x, y, x2, y2)) {
            player.set_stuck(floor.rooms[i].get_door('r') % 2, 0);
            player.set_stuck(floor.rooms[i].get_door('r') % 2 + 2, 0);
        }

    // Check whether enemies are adjacent to the player or not. Block player from moving from the tile the enemy is on.
    for (Enemy en : enemies) {
        int enx{ en.get_pos('x') }, eny{ en.get_pos('y') }, enx2{ enx + 40 }, eny2{ eny + 40 };

        if (y - 40 == eny && y2 - 40 == eny2 && x == enx && x2 == enx2)
            player.set_stuck(0, 1);
        if (x + 40 == enx && x2 + 40 == enx2 && y == eny && y2 == eny2)
            player.set_stuck(1, 1);
        if (y + 40 == eny && y2 + 40 == eny2 && x == enx && x2 == enx2)
            player.set_stuck(2, 1);
        if (x - 40 == enx && x2 - 40 == enx2 && y == eny && y2 == eny2)
            player.set_stuck(3, 1);
    }
}
void Interface::pl_atk() {
    if (enemies.size() == 0)
        return;

    sf::Vector2i mouse = sf::Mouse::getPosition(window);

    int x{ player.get_pos('x') }, y{ player.get_pos('y') }, x2{ x + 40 }, y2{ y + 40 };
    unsigned int pl_room_pos{ 255 }, en_room_pos{ 255 }, v{ 255 };

    // Check which room player is in and save that room index.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(x, y, x2, y2))
            pl_room_pos = i;

    // Check whether the mouse is clicked on an enemy. If not, return.
    for (unsigned int i{ 0 }; i < enemies.size(); i++)
        if (mouse.x >= enemies[i].get_pos('x') && mouse.x <= enemies[i].get_pos('x') + 40 && 
            mouse.y >= enemies[i].get_pos('y') && mouse.y <= enemies[i].get_pos('y') + 40)
            v = i;
    if (v == 255)
        return;

    // Check what room the selected enemy is in and save that room index.
    int enx{ enemies[v].get_pos('x') }, eny{ enemies[v].get_pos('y') }, enx2{ enx + 40 }, eny2{ eny + 40 };
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(enx, eny, enx2, eny2))
            en_room_pos = i;

    // Check whether the player and the enemy are in different room. If so, get either the player or enemy's room that has a door.
     unsigned int door_i = (floor.rooms[en_room_pos].door_exist()) ? en_room_pos : pl_room_pos;

    // If they are both in different room, check if they are adjacent to each other, separated only by a door. If not, return.
     if (pl_room_pos != en_room_pos) {
         if (floor.rooms[en_room_pos].door_exist() && floor.rooms[en_room_pos].touch_door(x, y, x2, y2) and
             floor.rooms[en_room_pos].touch_door(enx, eny, enx2, eny2)) {
         }
         else if (floor.rooms[pl_room_pos].door_exist() && floor.rooms[pl_room_pos].touch_door(x, y, x2, y2) and
             floor.rooms[pl_room_pos].touch_door(enx, eny, enx2, eny2)) {
         }
         else
             return;
     }

    log_add("You did "
        + std::to_string(enemies[v].set_hp(pl_weapon->get_stat(), player.get_stat(pl_weapon->get_stat()) + pl_weapon->get_quantity()))
        + " damage to the enemy.");
    Audio_Manager::play_sfx(2);
}
void Interface::refresh_exp() {
    main_level_text[1].setString(std::to_string(player.get_lvl()));
    main_level_text[2].setString(std::to_string(player.get_cur_exp()) + " / " + std::to_string(player.get_lvl_up()));

    if (player.get_cur_exp() < player.get_lvl_up())
        return;

    unsigned int carry_over_exp{ player.get_cur_exp() - player.get_lvl_up() };
    player.set_cur_exp(carry_over_exp);

    player.set_level(player.get_lvl() + 1);

    player.set_point(player.get_pts() + 3);

    player.set_lvl_up(10 + (player.get_lvl_up() / 10));

    main_level_text[1].setString(std::to_string(player.get_lvl()));
    main_level_text[2].setString(std::to_string(player.get_cur_exp()) + " / " + std::to_string(player.get_lvl_up()));
    log_add("You leveled up!");
}
void Interface::pl_sp_atk(Spell* spell) {
    std::array<int, 4> sp_inf = spell->atk(player);

    if (player.get_stat(7) < sp_inf[3])
        return;

    handle_sp_atk(sp_inf);
}
void Interface::pl_game_over() {
    if (player.is_dead()) {
        exit_menu = false;
        title = true;
        window.setView(viewUI);
        reset_game();
    }
}

// handle enemy events
void Interface::ene_add() {
    if (enemies.size() != 0 || enemy_respawn == 0)
        return;

    enemy_respawn -= 1;
    int rand_enemy_num{ rand() % 7 + 1 + static_cast<int>((player.get_floor() * 0.1)) };

    for (unsigned int z{ 0 }; z < rand_enemy_num; z++) {
        // Check which room player is not in.
        int rand_room{ rand() % static_cast<int>(floor.rooms.size()) };
        while (floor.rooms[rand_room].in_room(player.get_pos('x'), player.get_pos('y'),
            player.get_pos('x') + 40, player.get_pos('y') + 40))
            rand_room = rand() % floor.rooms.size();

        // Place in room.
        int temp_x{ -1 }, temp_y{ -1 }, counter{ 0 };
        while (true) {
            temp_x = ((rand() % (floor.rooms[rand_room].get_rm('w') / 40)) + (floor.rooms[rand_room].get_rm('x') / 40)) * 40,
            temp_y = ((rand() % (floor.rooms[rand_room].get_rm('h') / 40)) + (floor.rooms[rand_room].get_rm('y') / 40)) * 40;

            for (unsigned int i{ 0 }; i < enemies.size(); i++) {
                int tmp_x{ enemies[i].get_pos('x') }, tmp_y{ enemies[i].get_pos('y') };
                while ((temp_x == tmp_x && temp_y == tmp_y) || temp_x == -1 || temp_y == -1)
                    temp_x = ((rand() % (floor.rooms[rand_room].get_rm('w') / 40)) + (floor.rooms[rand_room].get_rm('x') / 40)) * 40,
                    temp_y = ((rand() % (floor.rooms[rand_room].get_rm('h') / 40)) + (floor.rooms[rand_room].get_rm('y') / 40)) * 40;
                counter += 1;
            }

            if (enemies.size() == counter)
                break;
        }
        int rand_enemy{ rand() % Texture_Manager::num_enemies };
        enemies.push_back(Enemy(-1, player.get_floor(), rand_enemy, temp_x, temp_y));
    }
}
void Interface::ene_overall() {
    for (unsigned int i{ 0 }; i < enemies.size(); i++)
        ene_action(i);
}
void Interface::ene_action(unsigned int v) {
    // Decide whether player is in range
    int x{ player.get_pos('x') }, y{ player.get_pos('y') }, x_2{ player.get_pos('x') + 40 }, y_2{ player.get_pos('y') + 40 },
    en_x{ enemies[v].get_pos('x') }, en_y{ enemies[v].get_pos('y') }, en_x2{ enemies[v].get_pos('x') + 40 }, en_y2{ enemies[v].get_pos('y') + 40 },
    chase_player_rand{rand() % 3};

    // If so, attack.
    if ((x >= en_x - (enemies[v].get_stat(4) * 40) && x_2 <= en_x && y == en_y) or
        (x_2 <= en_x2 + (enemies[v].get_stat(4) * 40) && x >= en_x2 && y == en_y) or
        (y_2 <= en_y && y >= en_y - (enemies[v].get_stat(4) * 40) && x == en_x) or
        (y_2 <= en_y2 + (enemies[v].get_stat(4) * 40) && y >= en_y2 && x == en_x))
        ene_atk(v);
    // If not in range but detected, move closer.
    else if (x >= en_x - 240 && y_2 <= en_y2 + 240 && chase_player_rand <= 1)
        ene_mov_close(v);
    // Otherwise, random movement or don't move at all.
    else
        ene_rand_move(v);
}
void Interface::ene_atk(unsigned int v) {
    int x{ player.get_pos('x') }, y{ player.get_pos('y') }, x2{ x + 40 }, y2{ y + 40 };
    unsigned int pl_room_pos{ 255 }, en_room_pos{ 255 };

    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(x, y, x2, y2))
            pl_room_pos = i;

    int enx{ enemies[v].get_pos('x') }, eny{ enemies[v].get_pos('y') }, enx2{enx + 40}, eny2{eny + 40};

    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(enx, eny, enx2, eny2))
            en_room_pos = i;

    // If they are both in different room, check if they are adjacent to each other, separated only by a door. If not, return.
    if (pl_room_pos != en_room_pos) {
        if (floor.rooms[en_room_pos].door_exist() && floor.rooms[en_room_pos].touch_door(x, y, x2, y2) and
            floor.rooms[en_room_pos].touch_door(enx, eny, enx2, eny2)) {}
        else if (floor.rooms[pl_room_pos].door_exist() && floor.rooms[pl_room_pos].touch_door(x, y, x2, y2) and
            floor.rooms[pl_room_pos].touch_door(enx, eny, enx2, eny2)) {}
        else
            return;
    }
    int damage = player.attack_pl(enemies[v].get_type(), pl_armor->get_quantity() + enemies[v].get_stat(1));
    log_add("The enemy did " + std::to_string(damage) + " damage to you.");
    Audio_Manager::play_sfx(2);
}
void Interface::ene_mov_close(unsigned int v) {
    int x{ player.get_pos('x') }, y{ player.get_pos('y') }, x2{ x + 40 }, y2{ y + 40 };

    int enx{ enemies[v].get_pos('x') }, eny{ enemies[v].get_pos('y') }, enx2{ enx + 40 }, eny2{ eny + 40 };

    unsigned int cur{ 255 }, return_now{ 0 };

    // Find which room the enemy is in.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(enx, eny, enx2, eny2))
            cur = i;

    // 0 = enemy can move, 1 = enemy touch wall, can't move, 2 = enemy touch door, can move
    if (x2 < enx && ene_mov_close_2(v, -40, 0)) {
        if (enx == floor.rooms[cur].get_rm('x')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx - 40, eny);
    }
    else if (y2 < eny && ene_mov_close_2(v, 0, -40)) {
        if (eny == floor.rooms[cur].get_rm('y')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx, eny - 40);
    }
    else if (x > enx2 && ene_mov_close_2(v, 40, 0)) {
        if (enx2 == floor.rooms[cur].get_rm('1')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx + 40, eny);
    }
    else if (y > eny2 && ene_mov_close_2(v, 0, 40)) {
        if (eny2 == floor.rooms[cur].get_rm('2')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx, eny + 40);
    }
}
bool Interface::ene_mov_close_2(unsigned int v, int offx, int offy) {
    // check for obstruction with other enemies and player
    int x{ enemies[v].get_pos('x') }, y{ enemies[v].get_pos('y') };

    for (unsigned int i{ 0 }; i < enemies.size(); i++) {
        if (v == i)
            continue;

        int enx{ enemies[i].get_pos('x') }, eny{ enemies[i].get_pos('y') };

        if (x + offx == enx && y + offy == eny)
            return false;
    }

    int plx{ player.get_pos('x') }, ply{ player.get_pos('y') };

    if (x + offx == plx && y + offy == ply)
        return false;
    return true;
}
void Interface::ene_rand_move(unsigned int v) {
    int enx{ enemies[v].get_pos('x') }, eny{ enemies[v].get_pos('y') }, enx2{ enx + 40 }, eny2{ eny + 40 };
    unsigned int cur{ 255 }, return_now{ 0 };

    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(enx, eny, enx2, eny2))
            cur = i;

    int rand_dir{ rand() % 5 };

    // 0 = enemy can move, 1 = enemy touch wall, can't move, 2 = enemy touch door, can move
    // 0 = left, 1 = up, 2 = right, 3 = down
    if (rand_dir == 0 && ene_mov_close_2(v, -40, 0)) {
        if (enx == floor.rooms[cur].get_rm('x')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx - 40, eny);
    }
    else if (rand_dir == 1 && ene_mov_close_2(v, 0, -40)) {
        if (eny == floor.rooms[cur].get_rm('y')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx, eny - 40);
    }
    else if (rand_dir == 2 && ene_mov_close_2(v, 40, 0)) {
        if (enx2 == floor.rooms[cur].get_rm('1')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx + 40, eny);
    }
    else if (rand_dir == 3 && ene_mov_close_2(v, 0, 40)) {
        if (eny2 == floor.rooms[cur].get_rm('2')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].set_pos(enx, eny + 40);
    }
}
void Interface::ene_dead() {
    if (enemies.size() == 0)
        return;

    for (int i{ static_cast<int>(enemies.size()) - 1 }; i > -1; i--)
        if (enemies[i].get_stat(0) <= 0) {
            player.set_cur_exp(player.get_cur_exp() + enemies[i].get_stat(5));
            enemies.erase(enemies.begin() + i);
            refresh_exp();
        }
}

// handle item/inventory events
void Interface::itm_equip_weapon(Item* weapon) {
    if (weapon->get_type() != 1)
        return;

    if (pl_weapon->get_type() != 3)
        pl_weapon->set_pos(weapon->get_pos('x'), weapon->get_pos('y'));

    pl_weapon = weapon;
    pl_weapon->set_pos(200, 140);
}
void Interface::itm_equip_armor(Item* armor) {
    if (armor->get_type() != 2)
        return;

    if (pl_armor->get_type() != 3)
        pl_armor->set_pos(armor->get_pos('x'), armor->get_pos('y'));

    pl_armor = armor;
    pl_armor->set_pos(600, 140);
}
void Interface::itm_add_item(Item* item) {
    if (items.size() == player.get_max_itm())
        return;

    items.push_back(item);

    int x{ 0 }, y{ 0 };

    for (unsigned int i{ 0 }; i < player.get_max_itm(); i++) {
        bool spot_taken{ false };
        x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (unsigned int j{ 0 }; j < items.size() - 1; j++)
            if (items[j]->get_pos('x') == x && items[j]->get_pos('y') == y) {
                spot_taken = true;
                break;
            }

        if (!spot_taken)
            item->set_pos(x, y);
    }
    itm_reoganize_inv();
}
void Interface::itm_reoganize_inv() {
    int x, y, z { -1 };

    for (unsigned int i{ 0 }; i < player.get_max_itm(); i++) {
        if (i == items.size())
            return;
        bool spot_taken{ false };

        x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (unsigned int j{ 0 }; j < items.size(); j++) {
            if (items[j] == pl_weapon || items[j] == pl_armor)
                continue;
            if (items[j]->get_pos('x') == x && items[j]->get_pos('y') == y) {
                spot_taken = true;
                break;
            }
            z = j;
        }

        if (!spot_taken && z != -1)
            items[z]->set_pos(x, y);
    }
}
void Interface::itm_select_shortcut(char place) {
    bool end_loop{ false };

    if (place == 's') {
        if (inv_select->get_id() != 0)
            for (unsigned int i{ 0 }; i < items.size(); i++)
                if (items[i]->get_type() == 0) {
                    select_itm = items[i];
                    cur_it_shortcut = i;
                    break;
                }

        inv_select = select_itm;
        inv_select_rect.setPosition(817, 305);
    }
    else if (place == 'r' || place == 'l') {
        if (items.size() == 2)
            return;

        int stop{ cur_it_shortcut };

        while (!end_loop) {
            cur_it_shortcut = ((place == 'l') ? cur_it_shortcut - 1 : cur_it_shortcut + 1) % items.size();

            if (items[cur_it_shortcut]->get_type() == 0) {
                select_itm = items[cur_it_shortcut];
                inv_select = select_itm;
                end_loop = true;
            }
            else if (stop == cur_it_shortcut)
                end_loop = true;
        }
    }
    else if (place == 'n') {
        for (unsigned int i{0}; i < items.size(); i++)
            if (items[i]->get_type() == 0) {
                select_itm = items[i];
                cur_it_shortcut = i;
                return;
            }
    }
}

// handle spell/spell menu events
void Interface::sp_add_spell(Spell* spell) {
    if (spells.size() == 10)
        return;

    spells.push_back(spell);

    int x{ 0 }, y{ 0 };

    for (unsigned int i{ 0 }; i < 10; i++) {
        bool spot_taken{ false };
        x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (unsigned int j{ 0 }; j < spells.size() - 1; j++) {
            if (spells[j]->get_pos('x') == x && spells[j]->get_pos('y') == y) {
                spot_taken = true;
                break;
            }
        }

        if (!spot_taken) {
            spell->set_pos(x, y);
            break;
        }
    }
}
void Interface::sp_reorganize() {
    int x{ 0 }, y{ 0 }, z{ 0 };

    for (unsigned int i{ 0 }; i < 10; i++) {
        bool spot_taken{ false };
        x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (unsigned int j{ 0 }; j < spells.size(); j++) {
            if (spells[j]->get_pos('x') == x && spells[j]->get_pos('y') == y) {
                spot_taken = true;
                break;
            }
        }

        if (!spot_taken) {
            spells[z]->set_pos(x, y);
            break;
        }
        z++;
    }
}
void Interface::sp_select_shortcut(char place) {
    bool end_loop{ false };

    if (place == 's') {
        for (unsigned int i{ 0 }; i < spells.size(); i++)
            if (spells[i]->get_id() != 0) {
                select_sp = spells[i];
                cur_sp_shortcut = i;
                break;
            }
        spell_select = select_sp;
        spell_select->set_pos(1037, 305);
        return;
    }

    if (place == 'l' || place == 'r') {
        int stop{ cur_sp_shortcut };

        while (!end_loop) {
            cur_sp_shortcut = ((place == 'l') ? cur_sp_shortcut - 1 : cur_sp_shortcut + 1) % spells.size();

            if (spells[cur_sp_shortcut]->get_id() != 0) {
                select_sp = spells[cur_sp_shortcut];
                spell_select = select_sp;
                end_loop = true;
            }
            else if (stop == cur_sp_shortcut)
                end_loop = true;
        }
    }
    else if (place == 'n') {
        for (unsigned int i{ 0 }; i < spells.size(); i++)
            if (spells[i]->get_id() != 0) {
                select_sp = spells[i];
                cur_sp_shortcut = i;
                return;
            }
    }
}

// handle text log event
void Interface::log_add(std::string string) {
    if (main_log.size() == 9) {
        main_log.erase(main_log.begin());
        for (unsigned int i = 0; i < main_log.size(); i++) {
            main_log[i].setPosition(755, main_log[i].getPosition().y - 30);
        }
    }
    main_log.push_back(sf::Text());
    main_log.back().setString(string);
    main_log.back().setFont(font);
    main_log.back().setOutlineColor(sf::Color::White);
    main_log.back().setCharacterSize(24);
    main_log.back().setPosition(755, 465 + main_log.size() * 30);
}

// handle drawing objects setup
void Interface::create_title_ui() {
    for (unsigned i{ 0 }; i < 5; i++) {
        sf::Text temp; sf::RectangleShape rect;

        if (i >= 2) {
            temp.setCharacterSize(48);
            temp.setPosition(350.f, 325.f + ((i - 2) * 75.f));

            rect.setSize(sf::Vector2f(110, 65));
            rect.setFillColor(sf::Color::Black);
            rect.setOutlineThickness(3.f);
            rect.setPosition(350.f, 325.f + ((i - 2) * 75.f));

            switch (i) {
            case 2:
                temp.setString("Start"); break;
            case 3:
                temp.setString("Load"); break;
            case 4:
                temp.setString("Exit"); break;
            }

            title_rectangle.push_back(rect);
        }
        else if (i == 0) {
            temp.setCharacterSize(96);
            temp.setPosition(170.f, 40.f);
            temp.setString("Roguelike\nDungeon");
        }
        else if (i == 1) {
            const char title_info[] =
                "Developed with the help of\ninternet forums and SFML Graphics,\nzlib/png license."
                "\n\nOpen Sans font, fontsource.org\n\nInstructions: use the arrow keys to "
                "\nmove the players. Tap on the level \nup button to upgrade your stats."
                "\n\nSound effects created on \nsfxr.me/, MIT License."
                "\n\nSongs created on \nbeepbox.co/, MIT License."
                "\n\nSprites created in Krita."
                "\n\nVersion: 3.0, with interactibles now.";

            temp.setPosition(740, 20);
            temp.setString(title_info);
            temp.setCharacterSize(24);

            rect.setSize(sf::Vector2f(450, 780));
            rect.setFillColor(sf::Color::Black);
            rect.setOutlineThickness(3.f);
            rect.setPosition(730, 10);
            title_rectangle.push_back(rect);
        }

        temp.setFont(font);
        temp.setStyle(sf::Text::Bold);

        title_texts.push_back(temp);
    }

    sf::Text sound_text;
    sound_text.setCharacterSize(48);
    sound_text.setPosition(10.f, 740.f);
    sound_text.setFont(font);
    sound_text.setStyle(sf::Text::Bold);
    sound_text.setString("Music: ON");
    title_texts.push_back(sound_text);
}
void Interface::create_name_ui() {
    new_game_prompt = sf::Text("Enter your name (up to 20 characters) and press 'Enter' after you are finished.", font, 24);
    new_game_prompt.setPosition(170, 150);
    new_game_prompt.setStyle(sf::Text::Bold);

    name_ph_2.setCharacterSize(24);
    name_ph_2.setPosition(470, 250);
    name_ph_2.setFont(font);
    name_ph_2.setStyle(sf::Text::Bold);
    name_ph_2.setString("Player");

    name_line.setSize(sf::Vector2f(270, 3));
    name_line.setPosition(470, 280);
    name_line.setFillColor(sf::Color::White);
}
void Interface::create_exit_ui() {
    for (unsigned i{ 0 }; i < 4; i++) {
        exit_texts.push_back(sf::Text("", font, 24));
        exit_rectangles.push_back(sf::RectangleShape());
        if (i == 0) {
            exit_rectangles[i].setSize(sf::Vector2f(800, 400));
            exit_rectangles[i].setOutlineThickness(3.f);
            exit_rectangles[i].setPosition(200, 200);

            exit_texts[i].setPosition(205.f, 220.f);
            exit_texts[i].setString("Any unsaved changes will be discarded. Do you still want to quit?");
        }
        else {
            exit_rectangles[i].setSize(sf::Vector2f(200, 40));
            exit_rectangles[i].setOutlineThickness(3.f);
            exit_rectangles[i].setPosition(480, 260.f + (80 * i));

            switch (i) {
            case 1:
                exit_texts[i].setString("Cancel");
                exit_texts[i].setPosition(sf::Vector2f(534, 263.f + (80 * i)));
                break;
            case 2:
                exit_texts[i].setString("Title Screen");
                exit_texts[i].setPosition(sf::Vector2f(498, 263.f + (80 * i)));
                break;
            case 3:
                exit_texts[i].setString("Quit");
                exit_texts[i].setPosition(sf::Vector2f(552, 263.f + (80 * i)));
                break;
            }
        }

        exit_texts[i].setStyle(sf::Text::Bold);
        exit_rectangles[i].setFillColor(sf::Color::Black);
    }
}
void Interface::create_stat_ui() {
    std::vector<std::string> stats_string{ "Health:", "Mana:", "Strength:", "Magic:", "Defense:", "Resistance:" };
    for (unsigned int i{ 0 }; i < 6; i++) {
        stats_draw.push_back(sf::Text());
        stats_text_draw.push_back(sf::Text());

        stats_text_draw[i].setCharacterSize(36);
        stats_text_draw[i].setString(stats_string[i]);
        stats_text_draw[i].setFont(font);
        stats_text_draw[i].setPosition(280.f, 250.f + (i * 50.f));

        stats_draw[i].setCharacterSize(36);
        stats_draw[i].setPosition(590, 250 + (i * 50));
        stats_draw[i].setFont(font);
    }

    for (unsigned int i{ 0 }; i < 12; i++) {
        if (i == 0) {
            stat_select_guide.setFont(font);
            stat_select_guide.setCharacterSize(42);
            stat_select_guide.setStyle(sf::Text::Bold);
            stat_select_guide.setPosition(50, 25);
            stat_select_guide.setString("Please allocate the player's stats. You have a limited \nnumber of stat points, so balance your stats carefully.\nYou can mess up your player if not done correctly.");
        }

        if (i < 2) {
            stats_left_draw.push_back(sf::Text());

            stats_left_draw[i].setFont(font);
            stats_left_draw[i].setCharacterSize(36);
            stats_left_draw[i].setStyle(sf::Text::Bold);
            stats_left_draw[i].setPosition(200 + (390 * i), 200);


            if (i == 0) stats_left_draw[i].setString("Stat points left: ");
        }

        stat_select_arrows.push_back(sf::Text());

        stat_select_arrows[i].setFont(font);
        stat_select_arrows[i].setCharacterSize(36);
        stat_select_arrows[i].setStyle(sf::Text::Bold);

        if (i % 2 == 0) {
            stat_select_arrows[i].setPosition(520, 250 + (i * 25));
            stat_select_arrows[i].setString("<");
        }
        else {
            stat_select_arrows[i].setPosition(660, 250 + ((i - 1) * 25));
            stat_select_arrows[i].setString(">");
        }
    }

    stat_select_continue_txt.setString("Continue");
    stat_select_continue_txt.setCharacterSize(42);
    stat_select_continue_txt.setFont(font);
    stat_select_continue_txt.setStyle(sf::Text::Bold);
    stat_select_continue_txt.setPosition(525, 580);

    stat_select_continue_rect.setSize(sf::Vector2f(190, 65));
    stat_select_continue_rect.setFillColor(sf::Color::Black);
    stat_select_continue_rect.setOutlineThickness(3.f);
    stat_select_continue_rect.setPosition(520, 580);
}
void Interface::create_main_ui() {
    std::vector<std::string> stats_string{ "Health:", "Mana:", "Strength:", "Magic:", "Defense:", "Resistance:" };
    for (unsigned int i{ 0 }; i < 6; i++) {
        if (i < 5) {
            main_border_vector.push_back(sf::RectangleShape());

            main_border_vector[i].setFillColor(sf::Color::Black);
            main_border_vector[i].setOutlineThickness(3.f);

            main_text.push_back(sf::Text());
            main_text[i].setFont(font);
            main_text[i].setStyle(sf::Text::Bold);
            main_text[i].setCharacterSize(24);
            main_text[i].setOutlineThickness(3.f);
            main_text[i].setOutlineColor(sf::Color::Black);

            switch (i) {
            case 0:
                main_border_vector[i].setSize(sf::Vector2f(200, 200));
                main_border_vector[i].setPosition(750, 250);
                main_text[i].setPosition(740, 10);
                main_text[i].setString("Name: ");
                main_text[i].setCharacterSize(16);
                break;
            case 1:
                main_border_vector[i].setSize(sf::Vector2f(200, 200));
                main_border_vector[i].setPosition(970, 250);
                main_text[i].setPosition(740, 50);
                main_text[i].setString("Stat: ");
                break;
            case 2:
                main_border_vector[i].setSize(sf::Vector2f(420, 300));
                main_border_vector[i].setPosition(750, 490);
                main_text[i].setPosition(740, 210);
                main_text[i].setString("Items: ");
                break;
            case 3:
                main_border_vector[i].setSize(sf::Vector2f(200, 100));
                main_border_vector[i].setPosition(970, 100);
                main_text[i].setPosition(950, 210);
                main_text[i].setString("Spells: ");
                break;
            case 4:
                main_text[i].setPosition(740, 455);
                main_text[i].setString("History: ");
                break;
            }
        }

        main_player_stats.push_back(sf::Text());
        main_player_stats_num.push_back(sf::Text());

        main_player_stats[i].setFont(font);
        main_player_stats[i].setCharacterSize(24);
        main_player_stats[i].setOutlineThickness(3.f);
        main_player_stats[i].setOutlineColor(sf::Color::Black);

        main_player_stats_num[i].setFont(font);
        main_player_stats_num[i].setCharacterSize(24);
        main_player_stats_num[i].setFillColor(sf::Color::Cyan);
        main_player_stats_num[i].setOutlineThickness(3.f);
        main_player_stats_num[i].setOutlineColor(sf::Color::Black);


        if (i < 2) {
            main_player_stats[i].setPosition(950, 10 + (i * 40));
            main_player_stats[i].setString(stats_string[i]);
            main_player_stats[i].setStyle(sf::Text::Bold);

            main_player_stats_num[i].setPosition(1050, 10 + (i * 40));
        }
        else {
            main_player_stats[i].setPosition(750, 80 + ((i - 2) * 30));
            main_player_stats[i].setString(stats_string[i]);

            main_player_stats_num[i].setPosition(900, 15 + (i * 30));
        }

        if (i < 3) {
            main_level_text.push_back(sf::Text());
            main_level_text[i].setFont(font);
            main_level_text[i].setCharacterSize(24);

            switch (i) {
            case 0:
                main_level_text[i].setPosition(980, 100);
                main_level_text[i].setString("Level");
                main_level_text[i].setStyle(sf::Text::Bold);
                break;
            case 1:
                main_level_text[i].setPosition(1060, 100);
                main_level_text[i].setFillColor(sf::Color::Cyan);
                break;
            case 2:
                main_level_text[i].setPosition(1040, 150);
                main_level_text[i].setFillColor(sf::Color::Cyan);
                break;
            }
        }
    }

    main_level_text[1].setString(std::to_string(player.get_lvl()));
    main_level_text[2].setString(std::to_string(player.get_cur_exp()) + " / " + std::to_string(player.get_lvl_up()));

    main_extra_menu_button.setString("...");
    main_extra_menu_button.setCharacterSize(42);
    main_extra_menu_button.setFont(font);
    main_extra_menu_button.setStyle(sf::Text::Bold);
    main_extra_menu_button.setPosition(1155, 730);
    main_extra_menu_button.setOutlineThickness(3.f);
    main_extra_menu_button.setOutlineColor(sf::Color(77, 160, 255));
    main_extra_menu_button.setFillColor(sf::Color(77, 160, 255));


    main_extra_menu_rect.setSize(sf::Vector2f(45, 45));
    main_extra_menu_rect.setFillColor(sf::Color::Black);
    main_extra_menu_rect.setOutlineThickness(3.f);
    main_extra_menu_rect.setPosition(1150, 750);
    main_extra_menu_rect.setOutlineColor(sf::Color(77, 160, 255));

    main_current_floor_txt.setCharacterSize(24);
    main_current_floor_txt.setFont(font);
    main_current_floor_txt.setStyle(sf::Text::Bold);
    main_current_floor_txt.setFillColor(sf::Color::Cyan);
    main_current_floor_txt.setPosition(10, 10);
    main_current_floor_txt.setOutlineThickness(3.f);
    main_current_floor_txt.setOutlineColor(sf::Color::Black);

    main_gold_txt.setCharacterSize(24);
    main_gold_txt.setFont(font);
    main_gold_txt.setStyle(sf::Text::Bold);
    main_gold_txt.setFillColor(sf::Color::Cyan);
    main_gold_txt.setPosition(10, 50);
    main_gold_txt.setOutlineThickness(3.f);
    main_gold_txt.setOutlineColor(sf::Color::Black);

    main_stat_popup_rect.setSize(sf::Vector2f(100, 150));
    main_stat_popup_rect.setFillColor(sf::Color::Black);
    main_stat_popup_rect.setOutlineThickness(2.f);
    main_stat_popup_rect.setOutlineColor(sf::Color::White);

    main_stat_popup_txt.setCharacterSize(16);
    main_stat_popup_txt.setFont(font);
    main_stat_popup_txt.setStyle(sf::Text::Bold);
    main_stat_popup_txt.setFillColor(sf::Color::Cyan);

    for (sf::Text &txt : shortcut_arrows) {
        txt.setCharacterSize(16);
        txt.setFont(font);
        txt.setStyle(sf::Text::Bold);
        txt.setFillColor(sf::Color::White);
    }

    shortcut_arrows[0].setPosition(765, 335);
    shortcut_arrows[0].setString("<");
    shortcut_arrows[1].setPosition(925, 335);
    shortcut_arrows[1].setString(">");
    shortcut_arrows[2].setPosition(985, 335);
    shortcut_arrows[2].setString("<");
    shortcut_arrows[3].setPosition(1145, 335);
    shortcut_arrows[3].setString(">");
}
void Interface::create_lvl_ui() {
    for (unsigned int i{ 0 }; i < 6; i++) {
        level_up_arrows.push_back(sf::Text());

        level_up_arrows[i].setFont(font);
        level_up_arrows[i].setCharacterSize(36);
        level_up_arrows[i].setStyle(sf::Text::Bold);
        level_up_arrows[i].setPosition(660, 250 + (i * 50));
        level_up_arrows[i].setString(">");
    }

    level_up_exit_txt.setFont(font);
    level_up_exit_txt.setCharacterSize(48);
    level_up_exit_txt.setStyle(sf::Text::Bold);
    level_up_exit_txt.setPosition(1128, 7);
    level_up_exit_txt.setString("X");

    level_up_exit_rect.setSize(sf::Vector2f(50, 50));
    level_up_exit_rect.setOutlineThickness(3.f);
    level_up_exit_rect.setPosition(1120, 10);
    level_up_exit_rect.setFillColor(sf::Color::Black);
}
void Interface::create_ext_ui() {
    extra_menu_black_bg.setSize(sf::Vector2f(200, 400));
    extra_menu_black_bg.setFillColor(sf::Color::Black);
    extra_menu_black_bg.setOutlineThickness(3.f);
    extra_menu_black_bg.setPosition(1000, 400);

    for (unsigned int i{ 0 }; i < 8; i++) {
        extra_menu_txt.push_back(sf::Text());
        extra_menu_rect.push_back(sf::RectangleShape());

        extra_menu_txt[i].setCharacterSize(24);
        extra_menu_txt[i].setFont(font);
        extra_menu_txt[i].setStyle(sf::Text::Bold);
        extra_menu_txt[i].setPosition(1010, 210 + (i * 100));
        extra_menu_txt[i].setFillColor(sf::Color(77, 160, 255));


        extra_menu_rect[i].setSize(sf::Vector2f(197, 99));
        extra_menu_rect[i].setOutlineThickness(3.f);
        extra_menu_rect[i].setFillColor(sf::Color::Black);
        extra_menu_rect[i].setPosition(1000, 200 + (i * 100));
        extra_menu_rect[i].setOutlineColor(sf::Color(77, 160, 255));

        switch (i) {
        case 0:
            extra_menu_txt[i].setString("Map");
            break;
        case 1:
            extra_menu_txt[i].setString("Load");
            break;
        case 2:
            extra_menu_txt[i].setString("Scan -\nDisabled");
            break;
        case 3:
            extra_menu_txt[i].setString("Equipment");
            break;
        case 4:
            extra_menu_txt[i].setString("Spells");
            break;
        case 5:
            extra_menu_txt[i].setString("Save");
            extra_menu_txt[i].setPosition(810, 710);
            extra_menu_rect[i].setPosition(800, 700);
            break;
        case 6:
            extra_menu_txt[i].setString("Help");
            extra_menu_txt[i].setPosition(10, 710);
            extra_menu_rect[i].setPosition(0, 700);
            break;
        }
    }

    extra_menu_help_txt.setCharacterSize(24);
    extra_menu_help_txt.setFont(font);
    extra_menu_help_txt.setStyle(sf::Text::Bold);
    extra_menu_help_txt.setPosition(110, 110);
    extra_menu_help_txt.setString("Arrow keys: Move player.\nW: Wait. Skip turn.\nQ: Go to the next floor "
        "if you are on the stair.\nEnter the shop if you are on it.\nESC: Bring up exit menu.\nClick on an "
        "enemy to attack them.\nThe Item/Spell slot gives you quick access to\nyour abilities. "
        "Click on the center of slot to use\nthe item.");
    extra_menu_help_txt.setFillColor(sf::Color(77, 160, 255));

    extra_menu_help_rect.setSize(sf::Vector2f(600, 400));
    extra_menu_help_rect.setOutlineThickness(3.f);
    extra_menu_help_rect.setFillColor(sf::Color::Black);
    extra_menu_help_rect.setOutlineColor(sf::Color(77, 160, 255));
    extra_menu_help_rect.setPosition(100, 100);
}
void Interface::create_inv_ui() {
    inv_title.setString("Equipment and Inventory");
    inv_title.setCharacterSize(42);
    inv_title.setFont(font);
    inv_title.setStyle(sf::Text::Bold);
    inv_title.setPosition(390, 20);

    inv_select_rect.setSize(sf::Vector2f(70, 70));
    inv_select_rect.setFillColor(sf::Color::Red);
    inv_select_rect.setOutlineThickness(3.f);
    inv_select_rect.setPosition(-100, -100);
    inv_select_rect.setOutlineColor(sf::Color::Red);

    for (unsigned int i{ 0 }; i < player.get_max_itm(); i++) {
        if (i < 4) {
            inv_equipment_slot_txt.push_back(sf::Text());
            inv_equipment_slot_rect.push_back(sf::RectangleShape());

            inv_equipment_slot_txt[i].setCharacterSize(24);
            inv_equipment_slot_txt[i].setFont(font);
            inv_equipment_slot_txt[i].setStyle(sf::Text::Bold);

            inv_equipment_slot_rect[i].setSize(sf::Vector2f(60, 60));
            inv_equipment_slot_rect[i].setFillColor(sf::Color::Black);
            inv_equipment_slot_rect[i].setOutlineThickness(3.f);

            switch (i) {
            case 0:
                inv_equipment_slot_rect[i].setPosition(200, 140);
                inv_equipment_slot_txt[i].setString("Weapon");
                inv_equipment_slot_txt[i].setPosition(200, 100);
                break;
            case 1:
                inv_equipment_slot_rect[i].setPosition(600, 140);
                inv_equipment_slot_txt[i].setString("Armor");
                inv_equipment_slot_txt[i].setPosition(600, 100);
                break;
            case 2:
                inv_equipment_slot_rect[i].setPosition(750, 140);
                inv_equipment_slot_txt[i].setString("Description");
                inv_equipment_slot_txt[i].setPosition(875, 100);
                inv_equipment_slot_rect[i].setFillColor(sf::Color::Transparent);
                inv_equipment_slot_rect[i].setSize(sf::Vector2f(400, 600));
                break;
            case 3:
                inv_equipment_slot_rect[i].setPosition(520, 660);
                inv_equipment_slot_txt[i].setString("Use Item");
                inv_equipment_slot_txt[i].setCharacterSize(36);
                inv_equipment_slot_txt[i].setPosition(530, 670);
                inv_equipment_slot_rect[i].setFillColor(sf::Color::Transparent);
                inv_equipment_slot_rect[i].setSize(sf::Vector2f(180, 80));
                break;
            }
        }

        inv_inventory_rect.push_back(sf::RectangleShape());

        inv_inventory_rect[i].setSize(sf::Vector2f(60, 60));
        inv_inventory_rect[i].setFillColor(sf::Color::Black);
        inv_inventory_rect[i].setOutlineThickness(3.f);

        inv_inventory_rect[i].setPosition(200 + ((i % 6) * 80), 240 + ((i / 6.0) * 80));
    }
}
void Interface::create_sp_ui() {
    spell_title.setString("Spells");
    spell_title.setCharacterSize(42);
    spell_title.setFont(font);
    spell_title.setStyle(sf::Text::Bold);
    spell_title.setPosition(460, 20);

    for (unsigned int i{ 0 }; i < 10; i++) {
        spell_slot_rect.push_back(sf::RectangleShape());

        spell_slot_rect[i].setSize(sf::Vector2f(60, 60));
        spell_slot_rect[i].setFillColor(sf::Color::Black);
        spell_slot_rect[i].setOutlineThickness(3.f);

        spell_slot_rect[i].setPosition(200 + ((i % 6) * 80), 240 + ((i / 6.0) * 80));

    }
}
void Interface::create_shop_ui() {
    unsigned int i{ 0 };

    for (sf::RectangleShape& rect : sh_slots) {
        rect.setSize(sf::Vector2f(60, 60));
        rect.setOutlineThickness(2.f);
        rect.setOutlineColor(sf::Color::White);
        rect.setFillColor(sf::Color::Black);

        rect.setPosition(200 + ((i % 6) * 80), 240 + ((i / 6.0) * 80));
        i++;
    }

    for (unsigned int j{ 0 }, i = 0; j < 3; j++, i++)
        sh_buy_sp[j]->set_pos(200 + ((i % 6) * 80), 240 + ((i / 6) * 80));

    for (unsigned int j{ 0 }, i = 0; j < 6; j++, i++)
        sh_buy_it[j]->set_pos(200 + ((i % 6) * 80), 240 + ((i / 6) * 80));

    for (sf::Text& txt : sh_buy_sell_txt) {
        txt.setPosition(545, 670);
        txt.setCharacterSize(36);
        txt.setFont(font);
    }

    sh_buy_sell_txt[0].setString("Buy");
    sh_buy_sell_txt[1].setString("Sell");

    i = 0;
    std::array<std::string, 4> sh_string{ "Item", "Spell", "Buy", "Sell"};
    for (sf::Text& txt : sh_type) {
        txt.setString(sh_string[i]);
        txt.setFont(font);
        txt.setCharacterSize(48);
        txt.setStyle(sf::Text::Underlined);

        (i % 2 == 0) ? txt.setPosition(350, 350) : txt.setPosition(750, 350);
        i++;
    }
}
void Interface::create_ld_ui() {
    ld_direction_txt.setString("Enter your character's name. The game will attempt to find the save file. Make sure\nthe save file (.sav) is in the same folder as the program executable.");
    ld_direction_txt.setCharacterSize(24);
    ld_direction_txt.setPosition(140, 150);
    ld_direction_txt.setFont(font);
    ld_direction_txt.setStyle(sf::Text::Bold);

    ld_savename_txt.setCharacterSize(24);
    ld_savename_txt.setPosition(470, 250);
    ld_savename_txt.setFont(font);
    ld_savename_txt.setStyle(sf::Text::Bold);
}

// handle draw to window
void Interface::draw_exit_screen() {
    window.setView(viewUI);
    for (unsigned i{ 0 }; i < 4; i++) {
        window.draw(exit_rectangles[i]);
        window.draw(exit_texts[i]);
    }
}
void Interface::draw_title_screen() {
    unsigned int size = title_texts.size();
    for (unsigned int i{ 0 }; i < size; i++) {
        if (i < 4)
            window.draw(title_rectangle[i]);
        window.draw(title_texts[i]);
    }
}
void Interface::draw_name_screen() {
    window.draw(new_game_prompt);
    window.draw(name_ph_2);
    window.draw(name_line);
}
void Interface::draw_main_ui() {
    window.setView(viewWorld);
    floor.draw(window);

    player.draw(window, 's');

    for (Enemy en : enemies)
        en.draw(window);

    window.setView(viewUI);
    for (sf::RectangleShape rect : main_border_vector)
        window.draw(rect);

    for (unsigned int i{ 0 }; i < 6; i++) {
        if (i < 5) {
            window.draw(main_text[i]);
        }
        window.draw(main_player_stats[i]);

        if (i == 0)
            main_player_stats_num[i].setString(std::to_string(player.get_stat(6)) + " / " + std::to_string(player.get_stat(i)));
        else if (i == 1)
            main_player_stats_num[i].setString(std::to_string(player.get_stat(7)) + " / " + std::to_string(player.get_stat(i)));
        else
            main_player_stats_num[i].setString(std::to_string(player.get_stat(i)));
        window.draw(main_player_stats_num[i]);
    }


    for (sf::Text txt : main_level_text)
        window.draw(txt);

    for (sf::Text txt : main_log)
        window.draw(txt);

    player.draw(window, 'n');
    window.draw(main_current_floor_txt);

    window.draw(main_gold_txt);

    window.draw(main_extra_menu_rect);
    window.draw(main_extra_menu_button);

    if (scan_popup) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        for (unsigned int i{ 0 }; i < enemies.size(); i++) {
            if (pos.x >= enemies[i].get_pos('x') && pos.x <= enemies[i].get_pos('x') + 40 && 
                pos.y >= enemies[i].get_pos('y') && pos.y <= enemies[i].get_pos('y') + 40) {
                main_stat_popup_rect.setPosition(enemies[i].get_pos('x') - 50, enemies[i].get_pos('y') - 50);
                main_stat_popup_txt.setPosition(enemies[i].get_pos('x') - 45, enemies[i].get_pos('y') - 45);


                main_stat_popup_txt.setString("HP: " + std::to_string(enemies[i].get_stat(0)) +
                    "\nATK: " + std::to_string(enemies[i].get_stat(1)) +
                    "\nDEF: " + std::to_string(enemies[i].get_stat(2)) +
                    "\nRES: " + std::to_string(enemies[i].get_stat(3)) +
                    "\nRAG: " + std::to_string(enemies[i].get_stat(4)));
                window.draw(main_stat_popup_rect);
                window.draw(main_stat_popup_txt);
                break;
            }
        }
    }

    window.draw(inv_select_rect);
    inv_select->draw(window, 's');
    spell_select->draw(window, 's');

    for (sf::Text txt : shortcut_arrows)
        window.draw(txt);
}
void Interface::draw_stat_selection() {
    stats_left_draw[1].setString(std::to_string(player.get_pts()));

    window.draw(stat_select_guide);
    window.draw(stat_select_continue_rect);
    window.draw(stat_select_continue_txt);

    for (unsigned int i{ 0 }; i < 12; i++) {
        if (i < 2)
            window.draw(stats_left_draw[i]);
        if (i < 6) {
            stats_draw[i].setString(std::to_string(player.get_stat(i)));
            window.draw(stats_text_draw[i]);
            window.draw(stats_draw[i]);
        }
        window.draw(stat_select_arrows[i]);
    }
}
void Interface::draw_lvl_ui() {
    stats_left_draw[1].setString(std::to_string(player.get_pts()));

    window.draw(stat_select_guide);
    window.draw(stat_select_continue_rect);
    window.draw(stat_select_continue_txt);

    for (unsigned int i{ 0 }; i < 6; i++) {
        if (i < 2) window.draw(stats_left_draw[i]);
        stats_draw[i].setString(std::to_string(player.get_stat(i)));
        window.draw(stats_text_draw[i]);
        window.draw(stats_draw[i]);
        window.draw(level_up_arrows[i]);

    }

    window.draw(level_up_exit_rect);
    window.draw(level_up_exit_txt);
}
void Interface::draw_ext_ui() {
    window.draw(extra_menu_black_bg);
    for (unsigned int i{ 0 }; i < extra_menu_txt.size(); i++) {
        window.draw(extra_menu_rect[i]);
        window.draw(extra_menu_txt[i]);
    }

    if (help_ui) {
        window.draw(extra_menu_help_rect);
        window.draw(extra_menu_help_txt);
    }
}
void Interface::draw_inv_ui() {
    window.draw(inv_title);
    window.draw(inv_select_rect);

    int item_button{ inv_select->get_type() == 0 ? 4 : 3 };

    for (unsigned int i{ 0 }; i < player.get_max_itm(); i++) {
        if (i < item_button) {
            window.draw(inv_equipment_slot_rect[i]);
            window.draw(inv_equipment_slot_txt[i]);
        }

        window.draw(inv_inventory_rect[i]);
    }

    window.draw(level_up_exit_rect);
    window.draw(level_up_exit_txt);

    inv_draw_desc->draw(window, 'd');
    for (Item* itm : items)
        itm->draw(window, 't');
}
void Interface::draw_sp_ui() {
    window.draw(spell_title);
    window.draw(inv_equipment_slot_rect[2]);
    window.draw(inv_equipment_slot_txt[2]);

    window.draw(inv_select_rect);

    for (sf::RectangleShape rect : spell_slot_rect)
        window.draw(rect);

    if (spell_select->get_id() != 0) {
        window.draw(inv_equipment_slot_rect[3]);
        window.draw(inv_equipment_slot_txt[3]);
    }

    window.draw(level_up_exit_rect);
    window.draw(level_up_exit_txt);

    spell_desc->draw(window, 'd');
    for (unsigned int i{ 0 }; i < spells.size(); i++)
        spells[i] ->draw(window, 't');
}
void Interface::draw_map_ui() {
    window.draw(background);
    floor.map.draw(window);
    window.draw(level_up_exit_rect);
    window.draw(level_up_exit_txt);
}

// handle button prompt
void Interface::name_creation(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            player.set_name(name_ph_2.getString());
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
            if (name_ph_1.size() != 0) {
                name_ph_1.pop_back();
                name_ph_2.setString(name_ph_1);
            }
            else
                name_ph_2.setString("");
        }
        else if (event.text.unicode < 128) {
            char letter{ static_cast<char>(event.text.unicode) };

            if (name_ph_1.size() > 20 || letter == '\\' || letter == ' ' || letter == '=' || 
                letter == '$' || letter == '*' || letter == '|' || letter == '~' || letter == '.' || 
                letter == '"' || letter == '\'')
                return;

            name_ph_1 += static_cast<char>(event.text.unicode);
            name_ph_2.setString(name_ph_1);
        }
    }
}
void Interface::handle_exit_prompt(int x, int y) {
    if (exit_menu) {
        if (x > 480 && x < 680 && y > 340 && y < 380)
            exit_menu = false;
        else if (x > 480 && x < 680 && y > 420 && y < 460) {
            exit_menu = false, title = true;
            reset_game();
        }
        else if (x > 480 && x < 680 && y > 500 && y < 540)
            window.close();
    }
}
void Interface::handle_stat_pick_prompt(int x, int y) {
    if (!exit_menu && stat_screen)
        for (unsigned int i{ 0 }; i < 6; i++) {
            if (y > 230 + (i * 57) && y < 270 + (i * 57)) {
                if (x > 480 && x < 560 && player.get_stat(i) != 0) {
                    if (i == 0 && player.get_stat(0) == 5) continue;
                    if (i <= 1) player.set_stat(6 + i, player.get_stat(6 + i) - 1);

                    player.set_stat(i, player.get_stat(i) - 1);
                    stats_draw[i].setString(std::to_string(player.get_stat(i)));
                    player.set_point(player.get_pts() + 1);
                    stats_left_draw[1].setString(std::to_string(player.get_pts()));
                }

                else if (x > 620 && x < 700 && player.get_pts() != 0) {
                    if (i <= 1) player.set_stat(6 + i, player.get_stat(6 + i) + 1);

                    player.set_stat(i, player.get_stat(i) + 1);
                    stats_draw[i].setString(std::to_string(player.get_stat(i)));
                    player.set_point(player.get_pts() - 1);
                    stats_left_draw[1].setString(std::to_string(player.get_pts()));
                }
            }
            else if (x > 520 && x < 710 && y > 580 && y < 645) {
                stat_screen = false, main_screen = true;
                player.set_point(0);
                stats_left_draw[1].setString("0");
            }
        }
}
void Interface::handle_lvl_prompt(int x, int y) {
    if (!exit_menu && lvl_screen) {
        if (!level_up_reset) {
            player.copy_stat(level_up_backup_stats);
            level_up_backup_left = player.get_pts();
            level_up_reset = true;
        }

        for (unsigned int i{ 0 }; i < 6; i++) {
            if (y > 230 + (i * 57) && y < 270 + (i * 57)) {
                if (x > 620 && x < 700 && player.get_pts() != 0) {
                    player.set_stat(i, player.get_stat(i) + 1);
                    stats_draw[i].setString(std::to_string(player.get_stat(i)));
                    player.set_point(player.get_pts() - 1);
                    stats_left_draw[1].setString(std::to_string(player.get_pts()));
                }
            }
            else if (x > 520 && x < 710 && y > 580 && y < 645) {
                lvl_screen = false, level_up_reset = false; main_screen = true;
                player.set_point(0);
                stats_left_draw[1].setString("0");
            }
        }

        if (x > 1120 && x < 1170 && y > 10 && y < 60) {
            player.copy_stat(level_up_backup_stats);
            player.set_point(level_up_backup_left);
            stats_left_draw[1].setString(std::to_string(player.get_pts()));
            lvl_screen = false, level_up_reset = false; main_screen = true;
        }
    }
}
void Interface::handle_title_prompt(int x, int y) {
    if (!exit_menu && title) {
        if (x > 350 && x < 460 && y > 475 && y < 535)
            window.close();
        else if (x > 350 && x < 460 && y > 325 && y < 385)
            title = false, name_screen = true;
        else if (x > 350 && x < 460 && y > 400 && y < 465)
            load();
        else if (x > 10 && x < 260 && y > 740 && y < 790) {
            Audio_Manager::set_music_volume(Audio_Manager::get_music_volume() ? 0 : 100);
            title_texts[title_texts.size() - 1].setString(Audio_Manager::get_music_volume() ? "Music: ON" : "Music: OFF");
        }
    }
}
void Interface::handle_main_prompt(int x, int y) {
    if (main_screen && !ext_screen) {
        if (x > 970 && x < 1170 && y > 100 && y < 200)
            lvl_screen = true, main_screen = false;
        else if (x > 1150 && x < 1200 && y > 750 && y < 800)
            ext_screen = true, main_screen = false;
        else if (x > 830 && x < 878 && y > 310 && y < 358 && inv_select->get_id() != 0) {
            std::vector<Item*>::iterator itr = std::find(items.begin(), items.end(), inv_select);
            items.erase(itr);
            inv_select = new Place_Holder(), select_itm = new Place_Holder();
            inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
            itm_reoganize_inv();
            ene_overall();
            itm_select_shortcut('n');
        }
        else if (x > 830 && x < 878 && y > 310 && y < 358 && inv_select->get_id() == 0)
            itm_select_shortcut('s');
        else if (x > 750 && x < 829 && y > 250 && y < 450 && items.size() > 1)
            itm_select_shortcut('l');
        else if (x > 879 && x < 960 && y > 250 && y < 450 && items.size() > 1)
            itm_select_shortcut('r');
        else if (x > 1050 && x < 1098 && y > 310 && y < 358 && spell_select->get_id() != 0) {
            (spell_select->get_use() != 4) ? spell_select->use(floor, player, enemies) : pl_sp_atk(spell_select);

            spell_select = new PH_Spell(), spell_desc = new PH_Spell();
            spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
            ene_overall();
        }
        else if (x > 1050 && x < 1098 && y > 310 && y < 358 && spell_select->get_id() == 0)
            sp_select_shortcut('s');
        else if (x > 970 && x < 1049 && y > 250 && y < 450 && spells.size() > 1)
            sp_select_shortcut('l');
        else if (x > 1099 && x < 1187 && y > 250 && y < 450 && spells.size() > 1)
            sp_select_shortcut('r');
    }
}
void Interface::handle_ext_prompt(int x, int y) {
    if (!exit_menu && ext_screen) {
        if (x > 1000 && x < 1200 && y > 200 && y < 300)
            map_menu = true, main_screen = ext_screen = false;
        else if (x > 1000 && x < 1200 && y > 300 && y < 400)
            load();
        else if (x > 1000 && x < 1200 && y > 400 && y < 500) {
            scan_popup = !scan_popup;
            scan_popup ? extra_menu_txt[2].setString("Scan -\nEnabled") : extra_menu_txt[2].setString("Scan -\nDisabled");
        }
        else if (x > 1000 && x < 1200 && y > 500 && y < 600) {
            ext_screen = false, main_screen = false, inv_screen = true, inv_select_rect.setPosition(-100, -100);
            inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder(); select_itm = new Place_Holder();
        }
        else if (x > 1000 && x < 1200 && y > 600 && y < 700)
            ext_screen = main_screen = false, sp_screen = true, inv_select_rect.setPosition(-100, -100);
        else if (x > 800 && x < 1000 && y > 700 && y < 800)
            save();
        else if (x > 0 && x < 200 && y > 700 && y < 800)
            help_ui = !help_ui;
        else if (!(x > 1000 && x < 1200 && y > 700 && y < 800))
            ext_screen = help_ui = false, main_screen = true;
    }
}
void Interface::handle_inv_prompt(int x, int y) {
    if (!exit_menu && inv_screen) {
        if (x > 1120 && x < 1170 && y > 10 && y < 60) {
            inv_screen = false, main_screen = true;
            inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder(); select_itm = new Place_Holder();
            inv_select_rect.setPosition(-100, -100);
        }

        else if (inv_select->get_type() == 0 && x > 520 && x < 700 && y > 660 && y < 740) {
            if (!inv_select->get_hp_mp_other())
                inv_select->use(player);

            std::vector<Item*>::iterator itr = std::find(items.begin(), items.end(), inv_select);
            items.erase(itr);
            inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder(); select_itm = new Place_Holder();
            inv_select->set_pos(-100, 100), inv_select_rect.setPosition(-100, -100);
            itm_reoganize_inv();
            ene_overall();
        }

        for (unsigned int i{ 0 }; i < items.size(); i++) {
            int i1x = items[i]->get_pos('x'), i1y = items[i]->get_pos('y');

            if (x > i1x && x < i1x + 60 && y > i1y && y < i1y + 60) {
                int i2x = inv_select->get_pos('x'), i2y = inv_select->get_pos('y');

                if (inv_select->get_type() != 3 && x > i2x && x < i2x + 60 && y > i2y && y < i2y + 60) {
                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if ((inv_select == pl_weapon && items[i]->get_type() == 1 && x > i1x && x < i1x + 60 && y > i1y && y < i1y + 60) or
                    (inv_select->get_type() == 1 && x > 200 && x < 260 && y > 140 && y < 200))  {
                    inv_select == pl_weapon ? itm_equip_weapon(items[i]) : itm_equip_weapon(inv_select);

                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if ((inv_select == pl_armor && items[i]->get_type() == 2 && x > i1x && x < i1x + 60 && y > i1y && y < i1y + 60) or
                    (inv_select->get_type() == 2 && x > 600 && x < 660 && y > 140 && y < 200)) {
                    inv_select == pl_armor ? itm_equip_armor(items[i]) : itm_equip_armor(inv_select);

                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (inv_select->get_type() != 3 && inv_select != pl_weapon && inv_select != pl_armor && 
                    !(x > 200 && x < 260 && y > 140 && y < 200) && !(x > 600 && x < 660 && y > 140 && y < 200)) {
                    inv_select->set_pos(items[i]->get_pos('x'), items[i]->get_pos('y')); 
                    items[i]->set_pos(i2x, i2y);
                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (inv_select->get_type() == 3) {
                    Audio_Manager::play_sfx(4);
                    inv_draw_desc = items[i];
                    inv_select = items[i];
                    inv_select_rect.setPosition(i1x - 5, i1y - 5);
                    return;
                }
            }
        }
    }
}
void Interface::handle_spell_prompt(int x, int y) {
    if (!exit_menu && sp_screen) {
        if (x > 1120 && x < 1170 && y > 10 && y < 60) {
            sp_screen = false, main_screen = true;
            spell_select = new PH_Spell(), spell_desc = new PH_Spell();
            inv_select_rect.setPosition(-100, -100);
        }

        if (spell_select->get_id() != 0 && x > 520 && x < 700 && y > 660 && y < 740) {

            (spell_select->get_use() != 4) ? spell_select->use(floor, player, enemies) : pl_sp_atk(spell_select);

            spell_select = new PH_Spell(), spell_desc = new PH_Spell();
            spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
            ene_overall();
        }

        for (unsigned int i{ 0 }; i < spells.size(); i++) {
            int sx = spells[i]->get_pos('x'), sy = spells[i]->get_pos('y');

            if (spell_select->get_id() != 0 && x > 520 && x < 700 && y > 660 && y < 740) {
                Audio_Manager::play_sfx(4);
                spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
            }

            if (x > sx && x < sx + 60 && y > sy && y < sy + 60) {
                int sx2 = spell_select->get_pos('x'), sy2 = spell_select->get_pos('y');

                if (spell_select->get_id() != 0 && x > sx2 && x < sx2 + 60 && y > sy2 && y < sy2 + 60) {
                    spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                    spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (spell_select->get_id() == 0) {
                    spell_desc = spells[i];
                    spell_select = spells[i];
                    inv_select_rect.setPosition(sx - 5, sy - 5);
                    return;
                }
            }
        }
    }
}
void Interface::handle_player_action(char input, unsigned int mode) {
    // mode: 0 = move, 1 = attack

    if (!mode) {
        bool handle = (input == 'u' || input == 'r' || input == 'd' || input == 'l');
        int offx{ 0 }, offy{ 0 };

        if (input == 'u')
            offy = -40;
        else if (input == 'd')
            offy = 40;

        if (input == 'l')
            offx = -40;
        else if (input == 'r')
            offx = 40;

        player.set_pos(player.get_pos('x') + offx, player.get_pos('y') + offy);
        viewWorld.move(offx, offy);
        window.setView(viewWorld);

        if (handle) {
            player.use_effect();
            ene_overall();
            handle_move_pick_itm();
            handle_move_pick_gld();
            handle_move_pick_interact();
        }
    }
    else if (mode == 1 && (input == 'u' || input == 'r' || input == 'd' || input == 'l')) {
        pl_atk();
        ene_dead();
        ene_overall();
        ene_add();
    }
}
void Interface::handle_move_pick_itm() {
    if (items.size() == player.get_max_itm())
        return;
    int pl_x{ player.get_pos('x') }, pl_y{ player.get_pos('y') };
    
    for (int i{ static_cast<int>(floor.collectibles.size()) - 1 }; i > -1; i--) {
        if (pl_x == floor.collectibles[i].get_pos('x') && pl_y == floor.collectibles[i].get_pos('y')) {
            Audio_Manager::play_sfx(0);
            itm_add_item(Item::create_itm(floor.collectibles[i].get_id(), font));
            floor.collectibles.erase(floor.collectibles.begin() + i);
            log_add("You picked up an item.");
            return;
        }
    }
}
void Interface::handle_move_pick_gld() {
    int plx{ player.get_pos('x') }, ply{ player.get_pos('y') };

    for (int i{ static_cast<int>(floor.golds.size()) - 1 }; i > -1; i--) {
        if (plx == floor.golds[i].get_pos('x') && ply == floor.golds[i].get_pos('y')) {
            Audio_Manager::play_sfx(0);
            int floor_gold = floor.golds[i].get_amount();
            player.set_gold(player.get_gold() + floor_gold);
            log_add("You collected " + std::to_string(floor_gold) + " golds.");
            main_gold_txt.setString("Gold: " + std::to_string(player.get_gold()));
            floor.golds.erase(floor.golds.begin() + i);
            return;

        }
    }
}
void Interface::handle_move_pick_interact() {
    int plx{ player.get_pos('x') }, ply{ player.get_pos('y') };

    for (int i{ static_cast<int>(floor.interactibles.size()) - 1 }; i > -1; i--) {
        if (plx == floor.interactibles[i].get_pos('x') && ply == floor.interactibles[i].get_pos('y')) {
            Audio_Manager::play_sfx(0);

            int effect = rand() % 100;
            if (effect < 10) {
                ene_add();
                log_add("More enemies are spawned.");
            }
            else if (effect >= 10 && effect < 20) {
                player.set_gold(player.get_gold() * .75);
                main_gold_txt.setString("Gold: " + std::to_string(player.get_gold()));
                log_add("Your golds decreased by 25%.");
            }
            else if (effect >= 20 && effect < 30) {
                player.set_stat(6, player.get_stat(6) * 1.5 > player.get_stat(0) ? player.get_stat(0) : player.get_stat(6) * 1.5);
                log_add("Your HP increased by 50%.");
            }
            else if (effect >= 30 && effect < 40) {
                player.set_stat(7, player.get_stat(7) * 1.5 > player.get_stat(1) ? player.get_stat(1) : player.get_stat(7) * 1.5);
                log_add("Your MP increased by 50%.");
            }
            else if (effect >= 40 && effect < 50) {
                ene_overall();
                ene_overall();
                log_add("You lost 2 turns.");
            }
            else if (effect >= 50 && effect < 60) {
                ene_overall();
                ene_overall();
                log_add("You will recover 1 health.");
            }
            else if (effect >= 60 && effect < 70) {
                player.set_effect(6, 1, 5);
                log_add("You will recover 1 HP for 5 turns.");
            }
            else if (effect >= 70 && effect < 80) {
                player.set_effect(2, -1, 7);
                log_add("You will lose 1 STR for 7 turns.");
            }
            else if (effect >= 80 && effect < 90) {
                player.set_effect(5, 2, 4);
                log_add("You will gain 2 RES for 4 turns.");
            }
            else
                log_add("Nothing happened.");

            floor.interactibles.erase(floor.interactibles.begin() + i);
            return;
        }
    }
}
void Interface::handle_map_prompt(int x, int y) {
    if (!exit_menu && map_menu) {
        if (x > 1120 && x < 1170 && y > 10 && y < 60)
            map_menu = main_screen = false, ext_screen = true;
        else
            floor.map.handle_event(x, y);
    }
}
void Interface::handle_sp_atk(std::array<int, 4> sp_inf) {
    inv_select_rect.setPosition(-100, -100);

    window.draw(background);
    draw_main_ui();
    window.draw(level_up_exit_rect);
    window.draw(level_up_exit_txt);
    window.display();

    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60)
                    return;

                if (x >= 400 - (sp_inf[1] * 40) && x <= 440 + (sp_inf[1] * 40) && y >= 400 - (sp_inf[1] * 40) && y <= 440 + (sp_inf[1] * 40))
                    for (unsigned int i{ 0 }; i < enemies.size(); i++) {
                        if (x >= enemies[i].get_pos('x') && x <= enemies[i].get_pos('x') + 40 and
                            y >= enemies[i].get_pos('y') && y <= enemies[i].get_pos('y') + 40) {
                            player.use_mp(sp_inf[3]);
                            enemies[i].set_hp(sp_inf[2], sp_inf[0] + player.get_stat(sp_inf[2] + 4));
                            return;
                        } else if (i == enemies.size() - 1)
                            return;
                    }
            }
        }
    }
}

// handle shop events
void Interface::handle_shop() {
    if (floor.get_shop_pos('x') != player.get_pos('x') || floor.get_shop_pos('y') != player.get_pos('y'))
        return;

    Audio_Manager::play_sfx(3);
    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60) {
                    main_gold_txt.setString("Gold: " + std::to_string(player.get_gold()));
                    return;
                }

                if (x > 350 && x < 500 && y > 350 && y < 400)
                    handle_sh_buy_and_sell(true);
                else if (x > 750 && x < 900 && y > 350 && y < 400)
                    handle_sh_buy_and_sell(false);
            }

        }

        window.clear();
        window.draw(background);

        window.draw(sh_type[2]);
        window.draw(sh_type[3]);

        window.draw(level_up_exit_rect);
        window.draw(level_up_exit_txt);
        window.display();
    }
}
void Interface::handle_sh_buy_and_sell(bool buy_or_sell) {
    window.clear();
    window.draw(background);

    window.draw(sh_type[0]);
    window.draw(sh_type[1]);

    window.draw(level_up_exit_rect);
    window.draw(level_up_exit_txt);
    window.display();

    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60)
                    return;

                if (x > 350 && x < 500 && y > 350 && y < 400)
                    (buy_or_sell) ? handle_sh_wp_buy() : handle_sh_wp_sell();
                else if (x > 750 && x < 900 && y > 350 && y < 400)
                    (buy_or_sell) ? handle_sh_sp_buy() : handle_sh_sp_sell();
            }
        }
    }
}
void Interface::handle_sh_wp_buy() {
    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60) {
                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (inv_select->get_id() != 0 && x > 520 && x < 700 && y > 660 && y < 740) {
                    if (player.use_gold(inv_select->get_buy_gd())) {
                        Item* it = new Item(*inv_select);
                        itm_add_item(it);
                        itm_reoganize_inv();
                    }
                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                }

                for (unsigned int i{ 0 }; i < 6; i++) {
                    int sx = sh_buy_it[i]->get_pos('x'), sy = sh_buy_it[i]->get_pos('y');

                    if (x > sx && x < sx + 60 && y > sy && y < sy + 60) {
                        int sx2 = inv_select->get_pos('x'), sy2 = inv_select->get_pos('y');

                        if (inv_select->get_id() != 0 && x > sx2 && x < sx2 + 60 && y > sy2 && y < sy2 + 60) {
                            inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                            inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                        }
                        else if (inv_select->get_id() == 0) {
                            inv_draw_desc = sh_buy_it[i];
                            inv_select = sh_buy_it[i];
                            inv_select_rect.setPosition(sx - 5, sy - 5);
                        }
                    }
                }
            }

        }

        window.clear();
        window.draw(background);
        window.draw(inv_title);

        window.draw(inv_select_rect);
        for (sf::RectangleShape rect : sh_slots)
            window.draw(rect);

        for (unsigned int i{ 0 }; i < 6; i++)
            sh_buy_it[i]->draw(window, 't');

        window.draw(inv_equipment_slot_rect[2]);
        window.draw(inv_equipment_slot_txt[2]);

        if (inv_select->get_id() != 0) {
            window.draw(inv_equipment_slot_rect[3]);
            window.draw(sh_buy_sell_txt[0]);
        }

        inv_select->draw(window, 'd');
        window.draw(level_up_exit_rect);
        window.draw(level_up_exit_txt);
        window.display();
    }
}
void Interface::handle_sh_wp_sell() {
    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60) {
                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (inv_select->get_id() != 0 && x > 520 && x < 700 && y > 660 && y < 740) {
                    player.set_gold(player.get_gold() + inv_select->get_sell_gd());

                    std::vector<Item*>::iterator itr = std::find(items.begin(), items.end(), inv_select);
                    items.erase(itr);
                    itm_reoganize_inv();
                    inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                    inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                }

                for (unsigned int i{ 0 }; i < items.size(); i++) {
                    int sx = items[i]->get_pos('x'), sy = items[i]->get_pos('y');

                    if (items[i] == pl_weapon || items[i] == pl_armor)
                        continue;

                    if (x > sx && x < sx + 60 && y > sy && y < sy + 60) {
                        int sx2 = inv_select->get_pos('x'), sy2 = inv_select->get_pos('y');

                        if (inv_select->get_id() != 0 && x > sx2 && x < sx2 + 60 && y > sy2 && y < sy2 + 60) {
                            inv_select = new Place_Holder(), inv_draw_desc = new Place_Holder();
                            inv_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                        }
                        else if (inv_select->get_id() == 0) {
                            inv_draw_desc = items[i];
                            inv_select = items[i];
                            inv_select_rect.setPosition(sx - 5, sy - 5);
                        }
                    }
                }
            }

        }

        window.clear();
        window.draw(background);
        window.draw(inv_title);

        window.draw(inv_select_rect);
        for (sf::RectangleShape rect : sh_slots)
            window.draw(rect);

        for (Item* itm : items)
            if (itm != pl_weapon && itm != pl_armor)
                itm->draw(window, 't');

        window.draw(inv_equipment_slot_rect[2]);
        window.draw(inv_equipment_slot_txt[2]);

        if (inv_select->get_id() != 0) {
            window.draw(inv_equipment_slot_rect[3]);
            window.draw(sh_buy_sell_txt[1]);
        }

        inv_select->draw(window, 'd');
        window.draw(level_up_exit_rect);
        window.draw(level_up_exit_txt);
        window.display();
    }
}
void Interface::handle_sh_sp_buy() {
    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60) {
                    spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                    spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (spell_select->get_id() != 0 && x > 520 && x < 700 && y > 660 && y < 740) {
                    
                    bool add_spell{ true };
                    for (unsigned int i{ 0 }; i < spells.size(); i++)
                        if (spell_select->get_id() == spells[i]->get_id()) {
                            add_spell = false;
                            break;
                        }
                    
                    if (add_spell)
                        if (player.use_gold(spell_select->get_buy_gd())) {
                            Spell* sp = new Spell(*spell_select);
                            sp_add_spell(sp);
                        }
                     
                    spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                    spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                }

                for (unsigned int i{ 0 }; i < 3; i++) {
                    int sx = sh_buy_sp[i]->get_pos('x'), sy = sh_buy_sp[i]->get_pos('y');

                    if (x > sx && x < sx + 60 && y > sy && y < sy + 60) {
                        int sx2 = spell_select->get_pos('x'), sy2 = spell_select->get_pos('y');

                        if (spell_select->get_id() != 0 && x > sx2 && x < sx2 + 60 && y > sy2 && y < sy2 + 60) {
                            spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                            spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                        }
                        else if (spell_select->get_id() == 0) {
                            spell_desc = sh_buy_sp[i];
                            spell_select = sh_buy_sp[i];
                            inv_select_rect.setPosition(sx - 5, sy - 5);
                        }
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(spell_title);

        window.draw(inv_select_rect);
        for (sf::RectangleShape rect : sh_slots)
            window.draw(rect);

        for (Spell* sp : sh_buy_sp)
            sp->draw(window, 't');

        window.draw(inv_equipment_slot_rect[2]);
        window.draw(inv_equipment_slot_txt[2]);

        if (spell_select->get_id() != 0) {
            window.draw(inv_equipment_slot_rect[3]);
            window.draw(sh_buy_sell_txt[0]);
        }

        spell_desc->draw(window, 'd');
        window.draw(level_up_exit_rect);
        window.draw(level_up_exit_txt);
        window.display();
    }
}
void Interface::handle_sh_sp_sell() {
    while (true) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x{ pos.x }, y{ pos.y };
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (x > 1120 && x < 1170 && y > 10 && y < 60) {
                    spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                    spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                    return;
                }
                else if (spell_select->get_id() != 0 && x > 520 && x < 700 && y > 660 && y < 740) {
                    player.set_gold(player.get_gold() + spell_select->get_sell_gd());

                    std::vector<Spell*>::iterator itr = std::find(spells.begin(), spells.end(), spell_select);
                    spells.erase(itr);
                    sp_reorganize();
                    spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                    spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                }

                for (unsigned int i{ 0 }; i < spells.size(); i++) {
                    int sx = spells[i]->get_pos('x'), sy = spells[i]->get_pos('y');

                    if (x > sx && x < sx + 60 && y > sy && y < sy + 60) {
                        int sx2 = spell_select->get_pos('x'), sy2 = spell_select->get_pos('y');

                        if (spell_select->get_id() != 0 && x > sx2 && x < sx2 + 60 && y > sy2 && y < sy2 + 60) {
                            spell_select = new PH_Spell(), spell_desc = new PH_Spell();
                            spell_select->set_pos(-100, -100), inv_select_rect.setPosition(-100, -100);
                        }
                        else if (spell_select->get_id() == 0) {
                            spell_desc = spells[i];
                            spell_select = spells[i];
                            inv_select_rect.setPosition(sx - 5, sy - 5);
                        }
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(spell_title);

        window.draw(inv_select_rect);
        for (sf::RectangleShape rect : sh_slots)
            window.draw(rect);

        for (Spell* sp : spells)
            sp->draw(window, 't');

        window.draw(inv_equipment_slot_rect[2]);
        window.draw(inv_equipment_slot_txt[2]);

        if (spell_select->get_id() != 0) {
            window.draw(inv_equipment_slot_rect[3]);
            window.draw(sh_buy_sell_txt[1]);
        }

        spell_desc->draw(window, 'd');
        window.draw(level_up_exit_rect);
        window.draw(level_up_exit_txt);
        window.display();
    }
}