/**
*
* File: game_manager.cpp
* Description: Contains the implementation of the Game_Manager class.
*
*/

#include "env.h"
#include "Manager/audio_manager.h"
#include "Manager/game_manager.h"
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
#include <Manager/game_state_manager.h>
#include <Manager/setting_manager.h>
#include <nfd.h>
#include <nlohmann/json.hpp>
#include <ostream>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <stat.h>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>
#include <Tool/tool.h>
#include <vector>

using json = nlohmann::json;

void Game_Manager::setup() {
    unsigned int i = 0;
    for (auto it = Item::items.begin(); it != Item::items.end(); it++, i++) {
        it->second.setPos(200 + ((i % 6) * 80), 240 + (static_cast<int>(i / 6.f) * 80.f));
        stockItem.push_back(it->second);
    }
    i = 0;
    for (auto it = Spell::spells.begin(); it != Spell::spells.end(); it++, i++) {
        it->second.setPos(200 + ((i % 6) * 80), 240 + (static_cast<int>(i / 6.f) * 80));
        stockSpell.push_back(it->second);
    }
    i = 0;
    for (auto it = Special::specials.begin(); it != Special::specials.end(); it++, i++) {
        it->second.setPos(200 + ((i % 6) * 80), 240 + (static_cast<int>(i / 6.f) * 80));
        stockSpecial.push_back(it->second);
    }
}

void Game_Manager::findItemShortcut(bool left) {
    unsigned int size = items.size();
    int stop{ itemQuickIndex };

    while (true) {
        itemQuickIndex = (itemQuickIndex + (left ? -1 : 1)) % size;
        if (items[itemQuickIndex].getType() != Weapon && items[itemQuickIndex].getType() != Armor) {
            selectedInv = itemQuickIndex;
            return;
        }
        else if (stop == itemQuickIndex)
            return;
    }
}

void Game_Manager::actEnemy() {
    delEnemy();
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
            atkEnemy(i);
        // If not in range but detected, move closer.
        else if (x >= en_x - 240 && y_2 <= en_y2 + 240 && chase_player_rand <= 1)
            moveEnemyClose(i);
        // Otherwise, random movement or don't move at all.
        else
            moveEnemyRand(i);
    }
}

void Game_Manager::findSpellShortcut(bool left) {
    unsigned int size = spells.size();
    if (size == 0)
        return;

    spellQuickIndex = (spellQuickIndex + (left ? -1 : 1)) % size;
    selectedSpell = spellQuickIndex;
}

void Game_Manager::atkEnemy(unsigned int v) {
    float x{ player.getPosition().x }, y{ player.getPosition().y }, x2{ x + 40 }, y2{ y + 40 };
    float enx{ enemies[v].getPosition().x }, eny{ enemies[v].getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };
    unsigned int pl_room_pos{ 255 }, en_room_pos{ 255 };
    Room* pl_room = NULL;
    Room* en_room = NULL;

    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++) {
        if (floor.rooms[i].inRoom(x, y, x2, y2))
            pl_room = &floor.rooms[i];
        if (floor.rooms[i].inRoom(enx, eny, enx2, eny2))
            en_room = &floor.rooms[i];
    }

    // If they are both in different room, check if they are adjacent to each other, separated only by a door. If not, return.
    if (pl_room != en_room &&
        !(pl_room->existDoor() && pl_room->touchDoor(x, y, x2, y2) && pl_room->touchDoor(enx, eny, enx2, eny2)) &&
        !(en_room->existDoor() && en_room->touchDoor(x, y, x2, y2) && en_room->touchDoor(enx, eny, enx2, eny2))
    )
        return;

    Attack type = enemies[v].constant->type;
    int quantity = plArmor.getQuantity();
    Stat armor_stat = plArmor.getStat();
    int armor = (type && armor_stat == Def) || (!type && armor_stat == Mgk) ? quantity : 0;
    int damage = player.hurtPlayer(type, enemies[v].stat.hp - armor);

    log_add(std::format("{} did {} damage to you.", enemies[v].constant->name, damage).c_str());
    Audio_Manager::playSFX(2);
}

void Game_Manager::moveEnemyClose(unsigned int v) {
    float x{ player.getPosition().x }, y{ player.getPosition().y }, x2{ x + 40 }, y2{ y + 40 };
    float enx{ enemies[v].getPosition().x }, eny{ enemies[v].getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };
    unsigned int cur{ 255 }, return_now{ 0 };

    // Find which room the enemy is in.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].inRoom(enx, eny, enx2, eny2))
            cur = i;

    // 0 = enemy can move, 1 = enemy touch wall, can't move, 2 = enemy touch door, can move
    if (x2 < enx && moveEnemyCloseH(v, -40, 0)) {
        if (enx == floor.rooms[cur].getRoom('x')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx - 40, eny);
    }
    else if (y2 < eny && moveEnemyCloseH(v, 0, -40)) {
        if (eny == floor.rooms[cur].getRoom('y')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny - 40);
    }
    else if (x > enx2 && moveEnemyCloseH(v, 40, 0)) {
        if (enx2 == floor.rooms[cur].getRoom('1')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx + 40, eny);
    }
    else if (y > eny2 && moveEnemyCloseH(v, 0, 40)) {
        if (eny2 == floor.rooms[cur].getRoom('2')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny + 40);
    }
}

bool Game_Manager::moveEnemyCloseH(unsigned int v, int offx, int offy) {
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

    return !(x + offx == plx && y + offy == ply);
}

void Game_Manager::moveEnemyRand(unsigned int v) {
    float enx{ enemies[v].getPosition().x }, eny{ enemies[v].getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };
    unsigned int cur{ 255 }, return_now{ 0 };

    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].inRoom(enx, eny, enx2, eny2))
            cur = i;

    int rand_dir{ rand() % 5 };

    // 0 = enemy can move, 1 = enemy touch wall, can't move, 2 = enemy touch door, can move
    // 0 = left, 1 = up, 2 = right, 3 = down
    if (rand_dir == 0 && moveEnemyCloseH(v, -40, 0)) {
        if (enx == floor.rooms[cur].getRoom('x')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx - 40, eny);
    }
    else if (rand_dir == 1 && moveEnemyCloseH(v, 0, -40)) {
        if (eny == floor.rooms[cur].getRoom('y')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny - 40);
    }
    else if (rand_dir == 2 && moveEnemyCloseH(v, 40, 0)) {
        if (enx2 == floor.rooms[cur].getRoom('1')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 1)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx + 40, eny);
    }
    else if (rand_dir == 3 && moveEnemyCloseH(v, 0, 40)) {
        if (eny2 == floor.rooms[cur].getRoom('2')) {
            return_now = 1;
            for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
                if (floor.rooms[i].touchDoor(enx, eny, enx2, eny2) && floor.rooms[i].getDoor('r') % 2 == 0)
                    return_now = 2;
        }

        if (return_now != 1)
            enemies[v].setPosition(enx, eny + 40);
    }
}

void Game_Manager::atkWithSpell(unsigned int enI) {
    Spell* selected = selectedSpell != SelectNone ? &spells[selectedSpell] : NULL;
    if (!selected)
        return;

    const int finalDamage = selected->getQuantity() + player.getStat(Mgk) * selected->getPercentage();

    player.useMP(selected->getMP());
    enemies[enI].stat.hp -= enemies[enI].stat.res >= finalDamage ? 1 : finalDamage - enemies[enI].stat.res;
    selectedSpell = SelectNone;
    actEnemy();
    log_add(std::format("Your spell did {} to {}.", finalDamage, enemies[enI].constant->name).c_str());
}

void Game_Manager::handlePlayerAct(sf::Keyboard::Key input, unsigned int mode) {
    bool handle = input >= sf::Keyboard::Left && input <= sf::Keyboard::Down;
    if (!handle)
        return;

    if (!mode) {
        int offx{ 0 }, offy{ 0 };

        switch (input) {
        case sf::Keyboard::Up:
            if (!player.isStuck(0))
                offy = -40;
            break;
        case sf::Keyboard::Down:
            if (!player.isStuck(2))
                offy = 40;
            break;
        case sf::Keyboard::Left:
            if (!player.isStuck(3))
                offx = -40;
            break;
        case sf::Keyboard::Right:
            if (!player.isStuck(1))
                offx = 40;
            break;
        }

        if (offx || offy) {
            player.setPosition(player.getPosition().x + offx, player.getPosition().y + offy);
            viewWorld.move(offx, offy);
            window.setView(viewWorld);

            handleTurn();
            pickUpItem();
            pickUpGold();
            stepOnInteractible();
        }
    }
    else {
        playerAttack();
        handleTurn();
    }
}

void Game_Manager::goUpFloor(bool bypass) {
    if (!bypass && !floor.intersectStair(player.getRect()))
        return;

    viewUI.reset(sf::FloatRect(0, 0, 1200, 800));
    viewWorld.reset(sf::FloatRect(0, 0, 1200, 800));
    viewSlots.reset(sf::FloatRect(0, 0, 1200, 800));

    Audio_Manager::playSFX(1);
    player.setFloor(player.getFloor() + 1);

    floor = Floor();
    playerRandomPos();
    enemies.clear();
    addEnemy();
    floor.makeCollectible(player.getFloor());
    floor.makeGold(player.getFloor());
    floor.makeInteractible(player.getFloor());

    centerFloor();
    enemyRespawns = 5;
    floorCopied = false;
}

void Game_Manager::pickUpItem() {
    if (items.size() == player.getMaxItems())
        return;

    sf::FloatRect rect = player.getRect();
    for (int i{ static_cast<int>(floor.collectibles.size()) - 1 }; i > -1; i--) {
        if (floor.collectibles[i].intersects(rect)) {
            Audio_Manager::playSFX(0);
            addTool(floor.collectibles[i].getID(), ItemTool);
            floor.collectibles.erase(floor.collectibles.begin() + i);
            log_add(std::format("You picked up: {}.", items[items.size() - 1].getName()).c_str());
            if (items.size() == player.getMaxItems())
                log_add("You reached your items limit.");
            return;
        }
    }
}

void Game_Manager::pickUpGold() {
    sf::FloatRect rect = player.getRect();
    for (int i{ static_cast<int>(floor.golds.size()) - 1 }; i > -1; i--) {
        if (floor.golds[i].intersects(rect)) {
            Audio_Manager::playSFX(0);
            int floor_gold = floor.golds[i].getGold();
            player.setGold(player.getGold() + floor_gold);
            log_add(std::format("You collected {} golds.", floor_gold).c_str());
            floor.golds.erase(floor.golds.begin() + i);
            return;
        }
    }
}

void Game_Manager::stepOnInteractible() {
    sf::FloatRect rect = player.getRect();
    for (int i{ static_cast<int>(floor.interactibles.size()) - 1 }; i >= 0; i--) {
        if (!floor.interactibles[i].inactive && floor.interactibles[i].intersects(rect)) {
            Audio_Manager::playSFX(0);

            int effect = INTERACTIBLE_CHANCE == 0 ? rand() % 100 : INTERACTIBLE_CHANCE;
            if (effect < 5) {
                addEnemy();
                log_add("More enemies are spawned.");
            }
            else if (effect >= 5 && effect < 10) {
                player.setGold(player.getGold() * .90);
                log_add("Your golds decreased by 10%.");
            }
            else if (effect >= 10 && effect < 15) {
                player.setGold(player.getGold() + 25);
                log_add("You found 25 golds!");
            }
            else if (effect >= 15 && effect < 20) {
                player.setStat(Hp, player.getStat(Hp) * 1.5 > player.getStat(Max_Hp) ? player.getStat(Max_Hp) : player.getStat(Hp) * 1.5);
                log_add("Your HP increased by 50%.");
            }
            else if (effect >= 20 && effect < 25) {
                player.setEffect(Def, 3, 3);
                log_add("You will gain 3 DEF for 3 turns.");
            }
            else if (effect >= 25 && effect < 30) {
                player.setEffect(Mgk, -2, 5);
                log_add("You will lose 2 MGK for 5 turns.");
            }
            else if (effect >= 30 && effect < 35) {
                player.setStat(Mp, player.getStat(Mp) * 1.5 > player.getStat(Max_Mp) ? player.getStat(Max_Mp) : player.getStat(Mp) * 1.5);
                log_add("Your MP increased by 50%.");
            }
            else if (effect >= 35 && effect < 40) {
                goUpFloor(true);
                log_add("You moved to the next floor.");
            }
            else if (effect >= 40 && effect < 45) {
                handleTurn();
                handleTurn();
                log_add("You lost 2 turns.");
            }
            else if (effect >= 45 && effect < 50) {
                player.setStat(Mp, player.getStat(Mp) < 8 ? 0 : player.getStat(Mp) - 8);
                log_add("You lose 8 MP.");
            }
            else if (effect >= 50 && effect < 55) {
                player.setEffect(Hp, 1, 5);
                log_add("You will recover 1 HP for 5 turns.");
            }
            else if (effect >= 55 && effect < 60) {
                player.setEffect(Res, -5, 2);
                log_add("You will lose 5 RES for 2 turns.");
            }
            else if (effect >= 60 && effect < 65) {
                player.setEffect(Hp, -1, 10);
                log_add("You will lose 1 HP for 10 turns.");
            }
            else if (effect >= 65 && effect < 70) {
                player.setEffect(Mp, -2, 5);
                log_add("You will lose 2 MP for 5 turns.");
            }
            else if (effect >= 70 && effect < 75) {
                addTool(2, ItemTool);
                log_add("You found a Rejuvenate Potion.");
            }
            else if (effect >= 75 && effect < 80) {
                player.setEffect(Str, -1, 7);
                log_add("You will lose 1 STR for 7 turns.");
            }
            else if (effect >= 80 && effect < 90) {
                player.setEffect(Res, 2, 4);
                log_add("You will gain 2 RES for 4 turns.");
            }
            else
                log_add("Nothing happened.");

            floor.interactibles[i].setInactive();
        }
    }
}

void Game_Manager::playerAttack() {
    if (enemies.size() == 0)
        return;

    float plx{ player.getPosition().x }, ply{ player.getPosition().y }, plx2{ plx + 40 }, ply2{ ply + 40 };
    Room* pl_room = NULL;
    Room* en_room = NULL;
    Enemy* en = NULL;

    // Check which room the player is in and save that room index.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].intersects(player.getRect()))
            pl_room = &floor.rooms[i];

    // Check whether the mouse is clicked on an enemy. If not, return.
    for (unsigned int i{ 0 }; i < enemies.size(); i++)
        if (enemies[i].contains(worldX, worldY))
            en = &enemies[i];
    if (en == NULL) return;

    // Check what room the selected enemy is in and save that room index.
    float enx{ en->getPosition().x }, eny{ en->getPosition().y }, enx2{enx + 40}, eny2{eny + 40};
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].inRoom(enx, eny, enx2, eny2))
            en_room = &floor.rooms[i];

    // If they are both in different room, check if they are adjacent to each other, separated only by a door. If not, return.
    if (pl_room != en_room &&
        !(en_room->existDoor() && en_room->touchDoor(plx, ply, plx2, ply2) && en_room->touchDoor(enx, eny, enx2, eny2)) &&
        !(pl_room->existDoor() && pl_room->touchDoor(plx, ply, plx2, ply2) && pl_room->touchDoor(enx, eny, enx2, eny2)))
            return;

    int stat{ en->constant->type ? en->stat.def : en->stat.res };
    const int amount = player.getStat(plWeapon.getStat()) + plWeapon.getQuantity();
    int quantity{ stat >= amount ? 1 : amount - stat };
    en->stat.hp -= quantity;

    log_add(std::format("You did {} damage to {}.",
        quantity,
        en->constant->name)
    .c_str());
    Audio_Manager::playSFX(2);
}

void Game_Manager::delEnemy() {
    if (enemies.size() == 0)
        return;

    for (int i{ static_cast<int>(enemies.size()) - 1 }; i > -1; i--)
        if (enemies[i].stat.hp <= 0) {
            player.setCurEXP(player.getCurEXP() + enemies[i].stat.exp);
            enemies.erase(enemies.begin() + i);
            updateEXP();
        }
}

void Game_Manager::organizeTool(ToolEnum type) {
    unsigned int size = items.size();
    unsigned int secondSize = player.getMaxItems();
    if (type == SpellTool) {
        size = spells.size();
        secondSize = MAX_INV_SPELL_SLOTS;
    }

    for (unsigned int i{ 0 }; i < secondSize && i < size; i++) {
        int x = 200 + ((i % 6) * 80), y = 240 + ((i / 6) * 80);
        (type == SpellTool) ? spells[i].setPos(x, y) : items[i].setPos(x, y);
    }
}

void Game_Manager::updateEXP() {
    const unsigned int lvlUpExp = player.getLVLUpEXP();

    if (player.getCurEXP() < lvlUpExp)
        return;

    unsigned int carry_over_exp{ player.getCurEXP() - lvlUpExp };
    player.setCurEXP(carry_over_exp);

    player.setLVL(player.getLVL() + 1);

    player.setStatPoint(player.getStatPts() + 5);

    player.setLVLUpEXP(10 + (lvlUpExp / 10.f * 1.5));

    log_add("You leveled up!");
}

void Game_Manager::playerRandomPos() {
    unsigned int i{ rand() % floor.rooms.size() };
    Room& room = floor.rooms[i];

    int temp_x = ((rand() % (room.getRoom('w') / 40)) + (room.getRoom('x') / 40)) * 40,
        temp_y = ((rand() % (room.getRoom('h') / 40)) + (room.getRoom('y') / 40)) * 40;

    if (PLACE_SHOP_ON_PLAYER) {
        temp_x = ((floor.getShopPos('x') + TILE / 2) / TILE) * TILE;
        temp_y = ((floor.getShopPos('y') + TILE / 2) / TILE) * TILE;
    }

    room.setVisisted();
    player.setPosition(temp_x, temp_y);
}

void Game_Manager::addEnemy() {
    if (enemies.size() != 0 || enemyRespawns == 0 || SKIP_SPAWN_ENEMY)
        return;

    enemyRespawns -= 1;
    int rand_enemy_num{ rand() % (7 + static_cast<int>((player.getFloor() * 0.01)))
        + 1 
        + static_cast<int>((player.getFloor() * 0.05)) };

    for (unsigned int z{ 0 }; z < rand_enemy_num; z++) {
        // Check which room player is not in.
        int rand_room{ rand() % static_cast<int>(floor.rooms.size()) };
        while (floor.rooms[rand_room].inRoom(player.getPosition().x, player.getPosition().y,
            player.getPosition().x + 40, player.getPosition().y + 40))
            rand_room = rand() % floor.rooms.size();
        Room& room = floor.rooms[rand_room];

        // Place in room.
        int temp_x{ -1 }, temp_y{ -1 }, counter{ 0 };
        while (true) {
            temp_x = ((rand() % (room.getRoom('w') / 40)) + (room.getRoom('x') / 40)) * 40,
                temp_y = ((rand() % (room.getRoom('h') / 40)) + (room.getRoom('y') / 40)) * 40;

            for (unsigned int i{ 0 }; i < enemies.size(); i++) {
                float tmp_x{ enemies[i].getPosition().x }, tmp_y{ enemies[i].getPosition().y };
                while ((temp_x == tmp_x && temp_y == tmp_y) || temp_x == -1 || temp_y == -1)
                    temp_x = ((rand() % (room.getRoom('w') / 40)) + (room.getRoom('x') / 40)) * 40,
                    temp_y = ((rand() % (room.getRoom('h') / 40)) + (room.getRoom('y') / 40)) * 40;
                counter += 1;
            }

            if (enemies.size() == counter)
                break;
        }

        auto it = Enemy::enemies.begin();
        while (player.getFloor() >= it->second.growth.minimumFloor)
            it = Enemy::enemies.begin();

        std::advance(it, rand() % Enemy::enemies.size());
        unsigned int id = it->first;
        enemies.push_back(Enemy(id, temp_x, temp_y, -1));
    }

    if (enemyRespawns == 0)
        log_add("No more enemies will spawn.");
}

void Game_Manager::centerFloor() {
    viewWorld.setCenter(player.getPosition().x, player.getPosition().y);
    viewMap.setCenter(player.getPosition().x, player.getPosition().y);
}

void Game_Manager::addTool(unsigned int id, ToolEnum type) {
    unsigned __int64 size = type == ItemTool ? items.size() : spells.size();
    unsigned __int64 x{ 200 + ((size % 6) * 80) }, y{ 240 + ((size / 6) * 80) };

    switch (type) {
    case ItemTool:
        if (size == player.getMaxItems()) {
            log_add("You reached your items limit.");
            return;
        }

        items.push_back(Item(id));
        items[size].setPos(x, y);
        return;
    case SpellTool:
        if (size == MAX_INV_SPELL_SLOTS)
            return;

        spells.push_back(Spell(id));
        spells[size].setPos(x, y);
        return;
    }
}

void Game_Manager::resetGame(bool cheat) {
    viewUI.reset(sf::FloatRect(0, 0, 1200, 800));
    viewWorld.reset(sf::FloatRect(0, 0, 1200, 800));
    viewSlots.reset(sf::FloatRect(0, 0, 1200, 800));
    worldZoomLevel = 0;

    player.reset(cheat);

    floor = Floor();
    playerRandomPos();
    enemies.clear();
    addEnemy();
    enemyRespawns = rand() % 5 + 1;
    floor.makeCollectible(player.getFloor());
    floor.makeGold(player.getFloor());
    floor.makeInteractible(player.getFloor());

    plWeapon = Item(5);
    plArmor = Item(8);
    plWeapon.setPos(200, 140);
    plArmor.setPos(600, 140);

    items.clear();
    addTool(0, ItemTool);
    addTool(1, ItemTool);

    addTool(6, ItemTool);
    addTool(9, ItemTool);

    spells.clear();
    addTool(5, SpellTool);

    organizeTool(ItemTool);
    findItemShortcut('n');

    centerFloor();
    logs.clear();
}

void Game_Manager::equipItem(unsigned int position, ItemType type) {
    if (type >= StatConsumable || type != items[position].getType())
        return;  

    unsigned int equippedItemID = type == Weapon ? plWeapon.getID() : plArmor.getID();

    addTool(equippedItemID, ItemTool);

    if (type == Weapon) {
        plWeapon = Item(items[position].getID());
        plWeapon.setPos(200, 140);
    }
    else {
        plArmor = Item(items[position].getID());
        plArmor.setPos(600, 140);
    }
    items.erase(items.begin() + position);

    organizeTool(ItemTool);
}

bool Game_Manager::gameOver() {
    if (player.isDead()) {
        window.setView(viewUI);
        resetGame();
        return true;
    }
    return false;
}

void Game_Manager::checkPlayerPath() {
    float x{ player.getPosition().x }, y{ player.getPosition().y }, x2{ x + 40 }, y2{ y + 40 };
    // Check which room player is in & determines if they're touching a wall.
    for (Room& rm : floor.rooms)
        if (rm.inRoom(x, y, x2, y2)) {
            (y == rm.getRoom('y')) ? player.setStuck(0, 1) : player.setStuck(0, 0);
            (x2 == rm.getRoom('1')) ? player.setStuck(1, 1) : player.setStuck(1, 0);
            (y2 == rm.getRoom('2')) ? player.setStuck(2, 1) : player.setStuck(2, 0);
            (x == rm.getRoom('x')) ? player.setStuck(3, 1) : player.setStuck(3, 0);
            rm.setVisisted();
            break;
        }

    // Check rooms that have doors & whether player is touching a door. It then allows player the ability to move through it.
    for (unsigned int i{ 0 }; i < floor.rooms.size(); i++)
        if (floor.rooms[i].touchDoor(x, y, x2, y2)) {
            player.setStuck(floor.rooms[i].getDoor('r') % 2, 0);
            player.setStuck(floor.rooms[i].getDoor('r') % 2 + 2, 0);
        }

    // Check whether enemies are adjacent to the player or not. Block player from moving from the tile the enemy is on.
    for (Enemy en : enemies) {
        float enx{ en.getPosition().x }, eny{ en.getPosition().y }, enx2{ enx + 40 }, eny2{ eny + 40 };

        if (y - 40 == eny && y2 - 40 == eny2 && x == enx && x2 == enx2)
            player.setStuck(0, 1);
        if (x + 40 == enx && x2 + 40 == enx2 && y == eny && y2 == eny2)
            player.setStuck(1, 1);
        if (y + 40 == eny && y2 + 40 == eny2 && x == enx && x2 == enx2)
            player.setStuck(2, 1);
        if (x - 40 == enx && x2 - 40 == enx2 && y == eny && y2 == eny2)
            player.setStuck(3, 1);
    }
}

void Game_Manager::save() {
    json j;
    try {
        j["name"] = player.getName();
        j["floorNumber"] = player.getFloor();
        j["level"] = player.getFloor();
        j["gold"] = player.getGold();
        j["statPoints"] = player.getStatPts();

        j["player"] = {
            {"x", player.getPosition().x},
            {"y", player.getPosition().y},
        };

        j["exp"] = {
            {"current", player.getCurEXP() },
            {"levelUp", player.getLVLUpEXP() }
        };
        j["inventorySlots"] = player.getMaxItems();

        j["stat"] = {
            {"hp",
                {
                    {"current", player.getStat(Hp)},
                    {"max", player.getStat(Max_Hp)}
                }
            },
            {"mp",
                {
                    {"current", player.getStat(Mp)},
                    {"max", player.getStat(Max_Mp)}
                }
            },
            {"str", player.getStat(Str)},
            {"mgk", player.getStat(Mgk)},
            {"def", player.getStat(Def)},
            {"res", player.getStat(Res)},
        };

        if (!items.empty())
            for (Item itm : items)
                j["inventory"].push_back(itm.getID());


        j["weaponID"] = plWeapon.getID();
        j["armorID"] = plArmor.getID();

        if (!spells.empty())
            for (Spell sp : spells)
                j["spell"].push_back(sp.getID());

        for (unsigned int i{ 0 }; i < floor.rooms.size(); i++) {
            j["floor"].push_back(json::object());
            j["floor"][i] = {
                {"x", floor.rooms[i].getRoom('x')},
                {"y", floor.rooms[i].getRoom('y')},
                {"sizeX", floor.rooms[i].getRoom('w')},
                {"sizeY", floor.rooms[i].getRoom('h')},
                {"visited", floor.rooms[i].getVisited()},
            };
            if (floor.rooms[i].existDoor()) {
                j["floor"][i]["door"] = {
                    {"x", floor.rooms[i].getDoor('x')},
                    {"y", floor.rooms[i].getDoor('y')},
                    {"rotation", floor.rooms[i].getDoor('r')},
                    {"door0", floor.rooms[i].getDoor('0')},
                    {"door1", floor.rooms[i].getDoor('1')},
                    {"door2", floor.rooms[i].getDoor('2')},
                    {"door3", floor.rooms[i].getDoor('3')},
                };
            }
        }

        j["stair"] = {
            {"x", floor.getStairPos('x')},
            {"y", floor.getStairPos('y')},
        };

        if (floor.isShopExist()) {
            j["shop"] = {
                {"x", floor.getShopPos('x')},
                {"y", floor.getShopPos('y')},
            };
        }

        j["enemiesRespawn"] = enemyRespawns;
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
                {"id", col.getID()},
                {"x", col.getPosition().x},
                {"y", col.getPosition().y},
            };
        }

        for (Gold_Collectible gold : floor.golds) {
            j["golds"].push_back(json::object());
            unsigned int i = j["golds"].size() - 1;
            j["golds"][i] = {
                {"amount", gold.getGold()},
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
                {"hidden", interact.getHidden()}
            };
        }

        for (const Effect& effect : player.getEffect()) {
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

    if (Setting_Manager::saveLocation.empty()) {
        nfdresult_t result = NFD_OpenDialog("sav", NULL, &outPath);

        if (result == NFD_CANCEL) {
            log_add("You closed the save dialog.");
            return;
        }
        else if (result != NFD_OKAY) {
            log_add("Errors occured while saving.");
            return;
        }
    }

    try {
        std::ofstream file_out{ Setting_Manager::saveLocation.empty() ? outPath : Setting_Manager::saveLocation };
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

bool Game_Manager::readSave() {
    nfdchar_t* outPath = NULL;

    if (Setting_Manager::saveLocation.empty()) {
        nfdresult_t result = NFD_OpenDialog("sav", NULL, &outPath);

        if (result == NFD_CANCEL) {
            printf("Error: User pressed cancel.");
            return false;
        }
        else if (result != NFD_OKAY) {
            printf("Error: %s\n", NFD_GetError());
            return false;
        }
    }

    try {
        std::ifstream file;
        file.exceptions(std::ifstream::badbit);
        file.open(Setting_Manager::saveLocation.empty() ? outPath : Setting_Manager::saveLocation);
        if (!file) {
            return false;
        }

        json j = json::parse(file);
        player.setName(j.at("name"));

        player.setFloor(j.at("floorNumber"));

        player.setPosition(j.at("player").at("x"), j.at("player").at("y"));

        player.setLVL(j.at("level"));
        player.setGold(j.at("gold"));
        player.setStatPoint(j.at("statPoints"));
        player.setCurEXP(j.at("exp").at("current"));
        player.setLVLUpEXP(j.at("exp").at("levelUp"));
        player.setMaxItem(j.at("inventorySlots"));

        player.setStat(Hp, j.at("stat").at("hp").at("current"));
        player.setStat(Max_Hp, j.at("stat").at("hp").at("max"));

        player.setStat(Mp, j.at("stat").at("mp").at("current"));
        player.setStat(Max_Mp, j.at("stat").at("mp").at("max"));

        player.setStat(Str, j.at("stat").at("str"));
        player.setStat(Mgk, j.at("stat").at("mgk"));
        player.setStat(Def, j.at("stat").at("def"));
        player.setStat(Res, j.at("stat").at("res"));

        items.clear();
        for (auto& element : j.at("inventory")) {
            addTool(element, ItemTool);
        }

        plWeapon = Item(j.at("weaponID"));
        plArmor = Item(j.at("armorID"));

        spells.clear();
        for (auto& element : j.at("spell")) {
            addTool(element, SpellTool);
        }

        floor = Floor(true);
        for (auto& room : j.at("floor")) {
            floor.loadRoom(room.at("x"), room.at("y"), room.at("sizeX"), room.at("sizeY"), room.at("visited"));
            if (room.contains("door")) {
                auto& door = room.at("door");
                floor.loadDoor(door.at("x"), door.at("y"), door.at("rotation"), door.at("door0"), door.at("door1"), door.at("door2"), door.at("door3"));
            }
        }

        floor.loadStair(j.at("stair").at("x"), j.at("stair").at("y"));
        if (j.contains("shop"))
            floor.loadStair(j.at("shop").at("x"), j.at("shop").at("y"));

        enemyRespawns = j.at("enemiesRespawn");
        enemies.clear();
        if (j.contains("enemies"))
            for (auto& enemy : j.at("enemies"))
                enemies.push_back(Enemy(enemy.at("id"), enemy.at("x"), enemy.at("y"), enemy.at("health")));

        if (j.contains("items"))
            for (auto& item : j.at("items"))
                floor.loadCollectible(item.at("x"), item.at("y"), item.at("id"));

        if (j.contains("golds"))
            for (auto& gold : j.at("golds"))
                floor.loadGold(gold.at("x"), gold.at("y"), gold.at("amount"));

        if (j.contains("interactibles"))
            for (auto& interactible : j.at("interactibles"))
                floor.loadInteractible(interactible.at("x"), interactible.at("y"), interactible.at("hidden"));

        player.resetEffect();

        if (j.contains("effects"))
            for (auto& effect : j.at("effects"))
                player.setEffect(effect.at("stat"), effect.at("difference"), effect.at("original"), effect.at("turn"));

        logs.clear();
        file.close();
        centerFloor();

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

void Game_Manager::delSelectedTool(ToolEnum type) {
    switch (type) {
    case SpellTool:
        spells.erase(spells.begin() + selectedSpell);
        selectedSpell = SelectNone;
        break;
    case ItemTool:
        items.erase(items.begin() + selectedInv);
        selectedInv = SelectNone;
        break;
    }
    organizeTool(type);
}

void Game_Manager::useItem() {
    ItemType type = (ItemType)items[selectedInv].getType();
    if (type != Weapon && type != Armor)
        items[selectedInv].use();

    delSelectedTool(ItemTool);
    handleTurn();
    selectedInv = SelectNone;
}

bool Game_Manager::useSpell() {
    if (spells[selectedSpell].getType() != Offensive) {
        bool success = spells[selectedSpell].use();
        selectedSpell = SelectNone;
        handleTurn();
        return success;
    }

    return false;
}

void Game_Manager::handleTurn() {
    player.useEffect();
    actEnemy();
    addEnemy();
}

Item* Game_Manager::getSelectedItem(std::vector<Item>& stocks)
{
    switch (selectedInv) {
    case SelectNone:
        return NULL;
    case SelectWeapon:
        return &plWeapon;
    case SelectArmor:
        return &plArmor;
    default:
        return &stocks[selectedInv];
    }

    return NULL;
}
