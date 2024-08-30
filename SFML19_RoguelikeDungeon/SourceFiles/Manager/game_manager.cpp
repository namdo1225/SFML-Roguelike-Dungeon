/**
*
* File: game_manager.cpp
* Description: Contains the implementation of the Game_Manager class.
*
*/

#include "env.h"
#include "Manager/audio_manager.h"
#include "Manager/game_manager.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <effect.h>
#include <exception>
#include <Floor/collectible.h>
#include <Floor/enemy.h>
#include <Floor/floor.h>
#include <Floor/gold_collectible.h>
#include <Floor/interactible.h>
#include <Floor/room.h>
#include <format>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <malloc.h>
#include <nlohmann/json.hpp>
#include <ostream>
#include <player.h>
#include <SFML/Graphics/Rect.hpp>
#include <stat.h>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>
#include <vector>
#include <nfd.h>

using json = nlohmann::json;

unsigned int Game_Manager::enemy_respawn = 5;
int Game_Manager::off_x = 0;
int Game_Manager::off_y = 0;

int Game_Manager::cur_it_shortcut = 0;
int Game_Manager::cur_sp_shortcut = 0;

Player Game_Manager::player;
Floor Game_Manager::floor = NULL;
std::vector<Enemy> Game_Manager::enemies;

std::vector<Item> Game_Manager::items;
std::vector<Spell> Game_Manager::spells;

Item* Game_Manager::pl_weapon = NULL;
Item* Game_Manager::pl_armor = NULL;

Item* Game_Manager::inv_select = NULL;
Spell* Game_Manager::spell_select = NULL;
Special* Game_Manager::special_select = NULL;

bool Game_Manager::floor_copied = false;

std::vector<Item> Game_Manager::item_stocks;
std::vector<Spell> Game_Manager::spell_stocks;
std::vector<Special> Game_Manager::special_stocks;

void Game_Manager::setup() {
    unsigned int i = 0;
    for (auto it = Item::items.begin(); it != Item::items.end(); it++, i++)
        it->second.setPos(200 + ((i % 6) * 80), 240 + ((i / 6.0f) * 80));
    i = 0;
    for (auto it = Spell::spells.begin(); it != Spell::spells.end(); it++, i++)
        it->second.setPos(200 + ((i % 6) * 80), 240 + ((i / 6.0f) * 80));
    i = 0;
    for (auto it = Special::specials.begin(); it != Special::specials.end(); it++, i++)
        it->second.setPos(200 + ((i % 6) * 80), 240 + ((i / 6.0f) * 80));
}

void Game_Manager::reorganize_inv() {
    int size = items.size();
    if (size <= 2)
        return;

    for (unsigned int i{ 0 }; i < player.get_max_itm() && i < size; i++) {
        int z{ 0 };
        int x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (; z < size; z++) {
            if (&items[z] == pl_weapon || &items[z] == pl_armor)
                continue;
            if (items[z].getPos('x') == x && items[z].getPos('y') == y) {
                z = -1;
                break;
            }
        }

        if (z != -1)
            items[--z].setPos(x, y);
    }
}

void Game_Manager::itm_select_shortcut(char place) {
    unsigned int size = items.size();
    if (size <= 2)
        return;

    switch (place) {
    case 's':
        for (unsigned int i{ 0 }; i < size; i++)
            if (items[i].type != Weapon && items[i].type != Armor) {
                inv_select = &items[i];
                cur_it_shortcut = i;
                return;
            }
        return;
    case 'r':
    case 'l':
        int stop{ cur_it_shortcut };

        while (true) {
            cur_it_shortcut = (cur_it_shortcut + (place == 'l' ? -1 : 1)) % size;
            if (items[cur_it_shortcut].type != Weapon && items[cur_it_shortcut].type != Armor) {
                inv_select = &items[cur_it_shortcut];
                return;
            }
            else if (stop == cur_it_shortcut)
                return;
        }
        break;
    }
}

void Game_Manager::ene_action() {
    ene_dead();
    for (unsigned int i{ 0 }; i < enemies.size(); i++) {
        // Decide whether player is in range
        float x{ player.getPosition().x }, y{ player.getPosition().y }, x_2{ x + 40 }, y_2{ y + 40 },
            en_x{ enemies[i].getPosition().x }, en_y{ enemies[i].getPosition().y }, en_x2{ en_x + 40 }, en_y2{ en_y + 40 };
        int chase_player_rand{ rand() % 3 };

        // If so, attack.
        int range = enemies[i].stat.range;
        if ((x >= en_x - (range * 40) && x_2 <= en_x && y == en_y) or
            (x_2 <= en_x2 + (range * 40) && x >= en_x2 && y == en_y) or
            (y_2 <= en_y && y >= en_y - (range * 40) && x == en_x) or
            (y_2 <= en_y2 + (range * 40) && y >= en_y2 && x == en_x))
            ene_atk(i);
        // If not in range but detected, move closer.
        else if (x >= en_x - 240 && y_2 <= en_y2 + 240 && chase_player_rand <= 1)
            ene_mov_close(i);
        // Otherwise, random movement or don't move at all.
        else
            ene_rand_move(i);
    }
}

void Game_Manager::sp_select_shortcut(char place) {
    unsigned int size = spells.size();
    if (size == 0)
        return;

    switch (place) {
    case 's':
        spell_select = &spells[0];
        cur_sp_shortcut = 0;
        return;
    case 'l':
    case 'r':
        cur_sp_shortcut = (cur_sp_shortcut + (place == 'l' ? -1 : 1)) % size;
        spell_select = &spells[cur_sp_shortcut];
        return;
    }
}

void Game_Manager::ene_atk(unsigned int v) {
    float x{ player.getPosition().x }, y{ player.getPosition().y }, x2{ x + 40 }, y2{ y + 40 };
    float enx{ enemies[v].getPosition().x }, eny{ enemies[v].getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };
    unsigned int pl_room_pos{ 255 }, en_room_pos{ 255 };
    Room* pl_room = NULL;
    Room* en_room = NULL;

    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++) {
        if (floor.rooms[i].in_room(x, y, x2, y2))
            pl_room = &floor.rooms[i];
        if (floor.rooms[i].in_room(enx, eny, enx2, eny2))
            en_room = &floor.rooms[i];
    }

    // If they are both in different room, check if they are adjacent to each other, separated only by a door. If not, return.
    if (pl_room != en_room &&
        !(pl_room->door_exist() && pl_room->touch_door(x, y, x2, y2) && pl_room->touch_door(enx, eny, enx2, eny2)) &&
        !(en_room->door_exist() && en_room->touch_door(x, y, x2, y2) && en_room->touch_door(enx, eny, enx2, eny2))
    )
        return;

    unsigned int enemy_type = enemies[v].stat.type;
    int quantity = pl_armor->quantity;
    Stat armor_stat = pl_armor->stat;
    int armor = (enemy_type && armor_stat == Def) || (!enemy_type && armor_stat == Mgk) ? quantity : 0;
    int damage = player.attack_pl(enemy_type, enemies[v].stat.hp - armor);

    log_add(std::format("{} did {} damage to you.", enemies[v].constant->name, damage).c_str());
    Audio_Manager::play_sfx(2);
}

void Game_Manager::ene_mov_close(unsigned int v) {
    float x{ player.getPosition().x }, y{ player.getPosition().y }, x2{ x + 40 }, y2{ y + 40 };
    float enx{ enemies[v].getPosition().x }, eny{ enemies[v].getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };
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
            enemies[v].setPosition(enx - 40, eny);
    }
    else if (y2 < eny && ene_mov_close_2(v, 0, -40)) {
        if (eny == floor.rooms[cur].get_rm('y')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny - 40);
    }
    else if (x > enx2 && ene_mov_close_2(v, 40, 0)) {
        if (enx2 == floor.rooms[cur].get_rm('1')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx + 40, eny);
    }
    else if (y > eny2 && ene_mov_close_2(v, 0, 40)) {
        if (eny2 == floor.rooms[cur].get_rm('2')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny + 40);
    }
}

bool Game_Manager::ene_mov_close_2(unsigned int v, int offx, int offy) {
    // check for obstruction with other enemies and player
    float x{ enemies[v].getPosition().x }, y{ enemies[v].getPosition().y };

    for (unsigned int i{ 0 }; i < enemies.size(); i++) {
        if (v == i)
            continue;

        float enx{ enemies[i].getPosition().x }, eny{ enemies[i].getPosition().y };

        if (x + offx == enx && y + offy == eny)
            return false;
    }

    float plx{ player.getPosition().x }, ply{ player.getPosition().y };

    if (x + offx == plx && y + offy == ply)
        return false;
    return true;
}

void Game_Manager::ene_rand_move(unsigned int v) {
    float enx{ enemies[v].getPosition().x }, eny{ enemies[v].getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };
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
            enemies[v].setPosition(enx - 40, eny);
    }
    else if (rand_dir == 1 && ene_mov_close_2(v, 0, -40)) {
        if (eny == floor.rooms[cur].get_rm('y')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny - 40);
    }
    else if (rand_dir == 2 && ene_mov_close_2(v, 40, 0)) {
        if (enx2 == floor.rooms[cur].get_rm('1')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx + 40, eny);
    }
    else if (rand_dir == 3 && ene_mov_close_2(v, 0, 40)) {
        if (eny2 == floor.rooms[cur].get_rm('2')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touch_door(enx, eny, enx2, eny2) && floor.rooms[i].get_door('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny + 40);
    }
}

void Game_Manager::pl_sp_atk(unsigned int en_i, std::array<int, 3> sp_inf) {
    player.use_mp(sp_inf[2]);
    enemies[en_i].stat.hp -= enemies[en_i].stat.res >= sp_inf[0] ? 1 : sp_inf[0] - enemies[en_i].stat.res;
    spell_select = NULL;
    ene_action();
    log_add(std::format("Your spell did {} to {}.", sp_inf[0], enemies[en_i].constant->name).c_str());
}

void Game_Manager::handle_player_action(char input, unsigned int mode) {
    // mode: 0 = move, 1 = attack
    bool handle = (input == 'u' || input == 'r' || input == 'd' || input == 'l');

    if (!mode) {
        int offx{ 0 }, offy{ 0 };

        if (input == 'u')
            offy = -40;
        else if (input == 'd')
            offy = 40;

        if (input == 'l')
            offx = -40;
        else if (input == 'r')
            offx = 40;

        player.setPosition(player.getPosition().x + offx, player.getPosition().y + offy);
        viewWorld.move(offx, offy);

        if (handle) {
            handle_turn();
            handle_move_pick_itm();
            handle_move_pick_gld();
            handle_move_pick_interact();
        }
    }
    else if (mode && handle) {
        pl_atk();
        handle_turn();
    }
}

void Game_Manager::next_level(bool bypass) {
    if (!bypass && !floor.stair_intersect(player.get_rect()))
        return;

    viewUI.reset(sf::FloatRect(0, 0, 1200, 800));
    viewWorld.reset(sf::FloatRect(0, 0, 1200, 800));

    Audio_Manager::play_sfx(1);
    player.set_floor(player.get_floor() + 1);

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

void Game_Manager::handle_move_pick_itm() {
    if (items.size() == player.get_max_itm())
        return;

    sf::FloatRect rect = player.get_rect();
    for (int i{ static_cast<int>(floor.collectibles.size()) - 1 }; i > -1; i--) {
        if (floor.collectibles[i].intersects(rect)) {
            Audio_Manager::play_sfx(0);
            add_item(floor.collectibles[i].get_id());
            floor.collectibles.erase(floor.collectibles.begin() + i);
            log_add(std::format("You picked up: {}.", items[items.size() - 1].name).c_str());
            if (items.size() == player.get_max_itm())
                log_add("You reached your items limit.");
            return;
        }
    }
}

void Game_Manager::handle_move_pick_gld() {
    sf::FloatRect rect = player.get_rect();
    for (int i{ static_cast<int>(floor.golds.size()) - 1 }; i > -1; i--) {
        if (floor.golds[i].intersects(rect)) {
            Audio_Manager::play_sfx(0);
            int floor_gold = floor.golds[i].get_amount();
            player.set_gold(player.get_gold() + floor_gold);
            log_add(std::format("You collected {} golds.", floor_gold).c_str());
            floor.golds.erase(floor.golds.begin() + i);
            return;
        }
    }
}

void Game_Manager::handle_move_pick_interact() {
    sf::FloatRect rect = player.get_rect();
    for (int i{ static_cast<int>(floor.interactibles.size()) - 1 }; i >= 0; i--) {
        if (!floor.interactibles[i].inactive && floor.interactibles[i].intersects(rect)) {
            Audio_Manager::play_sfx(0);

            int effect = INTERACTIBLE_CHANCE == 0 ? rand() % 100 : INTERACTIBLE_CHANCE;
            if (effect < 5) {
                ene_add();
                log_add("More enemies are spawned.");
            }
            else if (effect >= 5 && effect < 10) {
                player.set_gold(player.get_gold() * .90);
                log_add("Your golds decreased by 10%.");
            }
            else if (effect >= 10 && effect < 15) {
                player.set_gold(player.get_gold() + 25);
                log_add("You found 25 golds!");
            }
            else if (effect >= 15 && effect < 20) {
                player.set_stat(Hp, player.get_stat(Hp) * 1.5 > player.get_stat(Max_Hp) ? player.get_stat(Max_Hp) : player.get_stat(Hp) * 1.5);
                log_add("Your HP increased by 50%.");
            }
            else if (effect >= 20 && effect < 25) {
                player.set_effect(Def, 3, 3);
                log_add("You will gain 3 DEF for 3 turns.");
            }
            else if (effect >= 25 && effect < 30) {
                player.set_effect(Mgk, -2, 5);
                log_add("You will lose 2 MGK for 5 turns.");
            }
            else if (effect >= 30 && effect < 35) {
                player.set_stat(Mp, player.get_stat(Mp) * 1.5 > player.get_stat(Max_Mp) ? player.get_stat(Max_Mp) : player.get_stat(Mp) * 1.5);
                log_add("Your MP increased by 50%.");
            }
            else if (effect >= 35 && effect < 40) {
                next_level(true);
                log_add("You moved to the next floor.");
            }
            else if (effect >= 40 && effect < 45) {
                handle_turn();
                handle_turn();
                log_add("You lost 2 turns.");
            }
            else if (effect >= 45 && effect < 50) {
                player.set_stat(Mp, player.get_stat(Mp) < 8 ? 0 : player.get_stat(Mp) - 8);
                log_add("You lose 8 MP.");
            }
            else if (effect >= 50 && effect < 55) {
                player.set_effect(Hp, 1, 5);
                log_add("You will recover 1 HP for 5 turns.");
            }
            else if (effect >= 55 && effect < 60) {
                player.set_effect(Res, -5, 2);
                log_add("You will lose 5 RES for 2 turns.");
            }
            else if (effect >= 60 && effect < 65) {
                player.set_effect(Hp, -1, 10);
                log_add("You will lose 1 HP for 10 turns.");
            }
            else if (effect >= 65 && effect < 70) {
                player.set_effect(Mp, -2, 5);
                log_add("You will lose 2 MP for 5 turns.");
            }
            else if (effect >= 70 && effect < 75) {
                add_item(2);
                log_add("You found a Rejuvenate Potion.");
            }
            else if (effect >= 75 && effect < 80) {
                player.set_effect(Str, -1, 7);
                log_add("You will lose 1 STR for 7 turns.");
            }
            else if (effect >= 80 && effect < 90) {
                player.set_effect(Res, 2, 4);
                log_add("You will gain 2 RES for 4 turns.");
            }
            else
                log_add("Nothing happened.");

            floor.interactibles[i].setInactive();
        }
    }
}

void Game_Manager::pl_atk() {
    if (enemies.size() == 0)
        return;

    float plx{ player.getPosition().x }, ply{ player.getPosition().y }, plx2{ plx + 40 }, ply2{ ply + 40 };
    Room* pl_room = NULL;
    Room* en_room = NULL;
    Enemy* en = NULL;

    // Check which room the player is in and save that room index.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].intersects(player.get_rect()))
            pl_room = &floor.rooms[i];

    // Check whether the mouse is clicked on an enemy. If not, return.
    for (unsigned int i{ 0 }; i < enemies.size(); i++)
        if (enemies[i].contains(world_x, world_y))
            en = &enemies[i];
    if (en == NULL) return;

    // Check what room the selected enemy is in and save that room index.
    float enx{ en->getPosition().x }, eny{ en->getPosition().y }, enx2{enx + 40}, eny2{eny + 40};
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].in_room(enx, eny, enx2, eny2))
            en_room = &floor.rooms[i];

    // If they are both in different room, check if they are adjacent to each other, separated only by a door. If not, return.
    if (pl_room != en_room &&
        !(en_room->door_exist() && en_room->touch_door(plx, ply, plx2, ply2) && en_room->touch_door(enx, eny, enx2, eny2)) &&
        !(pl_room->door_exist() && pl_room->touch_door(plx, ply, plx2, ply2) && pl_room->touch_door(enx, eny, enx2, eny2)))
            return;

    int stat{ en->stat.type ? en->stat.def : en->stat.res };
    const int amount = player.get_stat(pl_weapon->stat) + pl_weapon->quantity;
    int quantity{ stat >= amount ? 1 : amount - stat };
    en->stat.hp -= quantity;

    log_add(std::format("You did {} damage to {}.",
        quantity,
        en->constant->name)
    .c_str());
    Audio_Manager::play_sfx(2);
}

void Game_Manager::ene_dead() {
    if (enemies.size() == 0)
        return;

    for (int i{ static_cast<int>(enemies.size()) - 1 }; i > -1; i--)
        if (enemies[i].stat.hp <= 0) {
            player.set_cur_exp(player.get_cur_exp() + enemies[i].stat.exp);
            enemies.erase(enemies.begin() + i);
            refresh_exp();
        }
}

void Game_Manager::refresh_exp() {
    const unsigned int lvlUpExp = player.get_lvl_up();

    if (player.get_cur_exp() < lvlUpExp)
        return;

    unsigned int carry_over_exp{ player.get_cur_exp() - lvlUpExp };
    player.set_cur_exp(carry_over_exp);

    player.set_level(player.get_lvl() + 1);

    player.set_point(player.get_pts() + 5);

    player.set_lvl_up(10 + (lvlUpExp / 10.f * 1.5));

    log_add("You leveled up!");
}

void Game_Manager::pl_random_pos() {
    unsigned int i{ rand() % floor.rooms.size() };
    Room& room = floor.rooms[i];

    int temp_x = ((rand() % (room.get_rm('w') / 40)) + (room.get_rm('x') / 40)) * 40,
        temp_y = ((rand() % (room.get_rm('h') / 40)) + (room.get_rm('y') / 40)) * 40;

    player.setPosition(temp_x, temp_y);
}

void Game_Manager::ene_add() {
    if (enemies.size() != 0 || enemy_respawn == 0 || SKIP_SPAWN_ENEMY)
        return;

    enemy_respawn -= 1;
    int rand_enemy_num{ rand() % 7 + 1 + static_cast<int>((player.get_floor() * 0.1)) };

    for (unsigned int z{ 0 }; z < rand_enemy_num; z++) {
        // Check which room player is not in.
        int rand_room{ rand() % static_cast<int>(floor.rooms.size()) };
        while (floor.rooms[rand_room].in_room(player.getPosition().x, player.getPosition().y,
            player.getPosition().x + 40, player.getPosition().y + 40))
            rand_room = rand() % floor.rooms.size();
        Room& room = floor.rooms[rand_room];

        // Place in room.
        int temp_x{ -1 }, temp_y{ -1 }, counter{ 0 };
        while (true) {
            temp_x = ((rand() % (room.get_rm('w') / 40)) + (room.get_rm('x') / 40)) * 40,
                temp_y = ((rand() % (room.get_rm('h') / 40)) + (room.get_rm('y') / 40)) * 40;

            for (unsigned int i{ 0 }; i < enemies.size(); i++) {
                float tmp_x{ enemies[i].getPosition().x }, tmp_y{ enemies[i].getPosition().y };
                while ((temp_x == tmp_x && temp_y == tmp_y) || temp_x == -1 || temp_y == -1)
                    temp_x = ((rand() % (room.get_rm('w') / 40)) + (room.get_rm('x') / 40)) * 40,
                    temp_y = ((rand() % (room.get_rm('h') / 40)) + (room.get_rm('y') / 40)) * 40;
                counter += 1;
            }

            if (enemies.size() == counter)
                break;
        }

        auto it = Enemy::enemies.begin();
        std::advance(it, rand() % Enemy::enemies.size());
        unsigned int id = it->first;
        enemies.push_back(Enemy(player.get_floor(), id, temp_x, temp_y));
    }

    if (enemy_respawn == 0)
        log_add("No more enemies will spawn.");
}

void Game_Manager::center_floor() {
    viewWorld.setCenter(player.getPosition().x, player.getPosition().y);
}

void Game_Manager::add_item(unsigned int id) {
    if (items.size() == player.get_max_itm()) {
        log_add("You reached your items limit.");
        return;
    }

    items.push_back(Item(id));

    int x{ 0 }, y{ 0 };

    for (unsigned int i{ 0 }; i < player.get_max_itm(); i++) {
        bool spot_taken{ false };
        x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (unsigned int j{ 0 }; j < items.size() - 1; j++)
            if (items[j].getPos('x') == x && items[j].getPos('y') == y) {
                spot_taken = true;
                break;
            }

        if (!spot_taken)
            items[items.size() - 1].setPos(x, y);
    }
    reorganize_inv();
}

void Game_Manager::reset_game(bool cheat) {
    viewUI.reset(sf::FloatRect(0, 0, 1200, 800));
    viewWorld.reset(sf::FloatRect(0, 0, 1200, 800));
    worldZoomLevel = 0;

    player.reset(cheat);

    floor = Floor();
    pl_random_pos();
    enemies.clear();
    ene_add();
    enemy_respawn = rand() % 5 + 1;
    floor.make_collectible(player.get_floor());
    floor.make_gold(player.get_floor());
    floor.make_interactible(player.get_floor());

    items.clear();
    add_item(5);
    add_item(8);
    add_item(0);
    add_item(1);

    spells.clear();
    add_spell(0);

    equip_weapon(&items[0]);
    equip_armor(&items[1]);
    reorganize_inv();
    itm_select_shortcut('n');

    center_floor();
    logs.clear();
}

void Game_Manager::add_spell(unsigned int id) {
    if (spells.size() == MAX_INV_SPELL_SLOTS)
        return;

    spells.push_back(Spell(id));

    int x{ 0 }, y{ 0 };

    for (unsigned int i{ 0 }; i < MAX_INV_SPELL_SLOTS; i++) {
        bool spot_taken{ false };
        x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (unsigned int j{ 0 }; j < spells.size() - 1; j++) {
            if (spells[j].getPos('x') == x && spells[j].getPos('y') == y) {
                spot_taken = true;
                break;
            }
        }

        if (!spot_taken) {
            spells[spells.size() - 1].setPos(x, y);
            break;
        }
    }
}

void Game_Manager::equip_weapon(Item* weapon) {
    if (weapon->type != Weapon)
        return;
    
    if (pl_weapon != NULL)
        pl_weapon->setPos(weapon->getPos('x'), weapon->getPos('y'));

    pl_weapon = weapon;
    pl_weapon->setPos(200, 140);
}

void Game_Manager::equip_armor(Item* armor) {
    if (armor->type != Armor)
        return;

    if (pl_armor != NULL)
        pl_armor->setPos(armor->getPos('x'), armor->getPos('y'));

    pl_armor = armor;
    pl_armor->setPos(600, 140);
}

bool Game_Manager::game_over() {
    if (player.is_dead()) {
        window.setView(viewUI);
        reset_game();
        return true;
    }
    return false;
}

void Game_Manager::pl_move_obstacle() {
    float x{ player.getPosition().x }, y{ player.getPosition().y }, x2{ x + 40 }, y2{ y + 40 };
    // Check which room player is in & determines if they're touching a wall.
    for (Room rm : floor.rooms)
        if (rm.in_room(x, y, x2, y2)) {
            (y == rm.get_rm('y')) ? player.set_stuck(0, 1) : player.set_stuck(0, 0);
            (x2 == rm.get_rm('1')) ? player.set_stuck(1, 1) : player.set_stuck(1, 0);
            (y2 == rm.get_rm('2')) ? player.set_stuck(2, 1) : player.set_stuck(2, 0);
            (x == rm.get_rm('x')) ? player.set_stuck(3, 1) : player.set_stuck(3, 0);
            break;
        }

    // Check rooms that have doors & whether player is touching a door. It then allows player the ability to move through it.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].touch_door(x, y, x2, y2)) {
            player.set_stuck(floor.rooms[i].get_door('r') % 2, 0);
            player.set_stuck(floor.rooms[i].get_door('r') % 2 + 2, 0);
        }

    // Check whether enemies are adjacent to the player or not. Block player from moving from the tile the enemy is on.
    for (Enemy en : enemies) {
        float enx{ en.getPosition().x }, eny{ en.getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };

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

void Game_Manager::save() {
    json j;
    try {
        j["name"] = player.get_name();
        j["floorNumber"] = player.get_floor();
        j["level"] = player.get_floor();
        j["gold"] = player.get_gold();
        j["statPoints"] = player.get_pts();

        j["player"] = {
            {"x", player.getPosition().x},
            {"y", player.getPosition().y},
        };

        j["exp"] = {
            {"current", player.get_cur_exp() },
            {"levelUp", player.get_lvl_up() }
        };
        j["inventorySlots"] = player.get_max_itm();

        j["stat"] = {
            {"hp",
                {
                    {"current", player.get_stat(Hp)},
                    {"max", player.get_stat(Max_Hp)}
                }
            },
            {"mp",
                {
                    {"current", player.get_stat(Mp)},
                    {"max", player.get_stat(Max_Mp)}
                }
            },
            {"str", player.get_stat(Str)},
            {"mgk", player.get_stat(Mgk)},
            {"def", player.get_stat(Def)},
            {"res", player.get_stat(Res)},
        };

        if (!items.empty())
            for (Item itm : items)
                j["inventory"].push_back(itm.id);


        for (int i = items.size() - 1; i >= 0; i--)
            if (&items[i] == pl_weapon) {
                j["weaponSlot"] = i;
                break;
            }
        for (int i = items.size() - 1; i >= 0; i--)
            if (&items[i] == pl_armor) {
                j["armorSlot"] = i;
                break;
            }

        if (!spells.empty())
            for (Spell sp : spells)
                j["spell"].push_back(sp.id);

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
                {"id", en.constant->id},
                {"health", en.stat.hp},
                {"x", en.getPosition().x},
                {"y", en.getPosition().y},
            };
        }

        for (Collectible col : floor.collectibles) {
            j["items"].push_back(json::object());
            unsigned int i = j["items"].size() - 1;
            j["items"][i] = {
                {"id", col.get_id()},
                {"x", col.getPosition().x},
                {"y", col.getPosition().y},
            };
        }

        for (Gold_Collectible gold : floor.golds) {
            j["golds"].push_back(json::object());
            unsigned int i = j["golds"].size() - 1;
            j["golds"][i] = {
                {"amount", gold.get_amount()},
                {"x", gold.getPosition().x},
                {"y", gold.getPosition().y},
            };
        }

        for (Interactible interact : floor.interactibles) {
            j["interactibles"].push_back(json::object());
            unsigned int i = j["interactibles"].size() - 1;
            j["interactibles"][i] = {
                {"x", interact.getPosition().x},
                {"y", interact.getPosition().y},
                {"hidden", interact.hidden}
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

    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog("sav", NULL, &outPath);

    if (result == NFD_CANCEL) {
        log_add("You closed the save dialog.");
        return;
    }
    else if (result != NFD_OKAY) {
        log_add("Errors occured while saving.");
        return;
    }

    try {
        std::ofstream file_out{ outPath };
        if (!file_out) {
            log_add("Save not successful.");
            return;
        }

        file_out << std::setw(4) << j << std::endl;
        file_out.close();
        free(outPath);
        log_add("Saved successfully.");
    }
    catch (const std::exception&) {
        log_add("Save not successful.");
    }
}

bool Game_Manager::read_save() {
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog("sav", NULL, &outPath);

    if (result == NFD_CANCEL) {
        printf("Error: User pressed cancel.");
        return false;
    }
    else if (result != NFD_OKAY) {
        printf("Error: %s\n", NFD_GetError());
        return false;
    }

    try {
        std::ifstream file;
        file.exceptions(std::ifstream::badbit);
        file.open(outPath);
        if (!file) {
            return false;
        }

        json j = json::parse(file);
        player.set_name(j.at("name"));

        player.set_floor(j.at("floorNumber"));

        player.setPosition(j.at("player").at("x"), j.at("player").at("y"));

        player.set_level(j.at("level"));
        player.set_gold(j.at("gold"));
        player.set_point(j.at("statPoints"));
        player.set_cur_exp(j.at("exp").at("current"));
        player.set_lvl_up(j.at("exp").at("levelUp"));
        player.set_max_item(j.at("inventorySlots"));

        player.set_stat(Hp, j.at("stat").at("hp").at("current"));
        player.set_stat(Max_Hp, j.at("stat").at("hp").at("max"));

        player.set_stat(Mp, j.at("stat").at("mp").at("current"));
        player.set_stat(Max_Mp, j.at("stat").at("mp").at("max"));

        player.set_stat(Str, j.at("stat").at("str"));
        player.set_stat(Mgk, j.at("stat").at("mgk"));
        player.set_stat(Def, j.at("stat").at("def"));
        player.set_stat(Res, j.at("stat").at("res"));

        items.clear();
        for (auto& element : j.at("inventory")) {
            add_item(element);
        }
        equip_weapon(&items[j.at("weaponSlot")]);
        equip_armor(&items[j.at("armorSlot")]);

        spells.clear();
        for (auto& element : j.at("spell")) {
            add_spell(element);
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
                floor.load_interactible(interactible.at("x"), interactible.at("y"), interactible.at("hidden"));

        player.reset_effect();

        if (j.contains("effects"))
            for (auto& effect : j.at("effects"))
                player.set_effect(effect.at("stat"), effect.at("difference"), effect.at("original"), effect.at("turn"));

        logs.clear();
        file.close();
        center_floor();

        log_add("The maze changed its appearance...");
        log_add("Save loaded.");

        free(outPath);
    }
    catch (const std::exception&) {
        printf("Error while trying to open save file.");
        return false;
    }
    return true;
}

void Game_Manager::reorganize_spell() {
    int size = spells.size();

    for (unsigned int i{ 0 }; i < MAX_INV_SPELL_SLOTS && i < size; i++) {
        int z{ 0 };
        int x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);

        for (; z < size; z++) {
            if (spells[z].getPos('x') == x && spells[z].getPos('y') == y) {
                z = -1;
                break;
            }
        }

        if (z != -1) {
            spells[--z].setPos(x, y);
        }
    }
}

void Game_Manager::delete_selected_itm() {    
    for (int i = items.size() - 1; i >= 0; i--)
        if (&items[i] == inv_select) {
            items.erase(items.begin() + i);
            return;
        }

    inv_select = NULL;
    reorganize_inv();
}

void Game_Manager::deleted_selected_sp() {
    for (int i = spells.size() - 1; i >= 0; i--)
        if (&spells[i] == spell_select) {
            spells.erase(spells.begin() + i);
            return;
        }

    spell_select = NULL;
    reorganize_spell();
}

void Game_Manager::item_use() {
    if (inv_select->type != Weapon && inv_select->type != Armor)
        inv_select->use();

    delete_selected_itm();
    handle_turn();
}

bool Game_Manager::spell_use() {
    if (spell_select->type != Offensive) {
        bool success = spell_select->use();
        spell_select = NULL;
        handle_turn();
        return success;
    }

    return false;
}

void Game_Manager::handle_turn() {
    player.use_effect();
    ene_action();
    ene_add();
}