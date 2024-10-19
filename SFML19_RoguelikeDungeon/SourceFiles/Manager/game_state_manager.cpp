#include "Manager/game_state_manager.h"
#include <Floor/enemy.h>
#include <Floor/floor.h>
#include <player.h>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>
#include <vector>

unsigned int Game_State_Manager::enemyRespawns = 5;
int Game_State_Manager::offX = 0;
int Game_State_Manager::offY = 0;

int Game_State_Manager::itemQuickIndex = 0;
int Game_State_Manager::spellQuickIndex = 0;

Player Game_State_Manager::player;
Floor Game_State_Manager::floor = NULL;
std::vector<Enemy> Game_State_Manager::enemies;

std::vector<Item> Game_State_Manager::items;
std::vector<Spell> Game_State_Manager::spells;

Item Game_State_Manager::plWeapon = Item();
Item Game_State_Manager::plArmor = Item();

int Game_State_Manager::selectedInv = SelectNone;
int Game_State_Manager::selectedSpell = SelectNone;
int Game_State_Manager::selectedSpecial = SelectNone;

bool Game_State_Manager::floorCopied = false;

std::vector<Item> Game_State_Manager::stockItem;
std::vector<Spell> Game_State_Manager::stockSpell;
std::vector<Special> Game_State_Manager::stockSpecial;
