#include "State/game_state.h"
#include <Floor/enemy.h>
#include <player.h>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>
#include <vector>

unsigned int Game_State::enemyRespawns = 5;
int Game_State::offX = 0;
int Game_State::offY = 0;

int Game_State::itemQuickIndex = 0;
int Game_State::spellQuickIndex = 0;

std::vector<Enemy> Game_State::enemies;

std::vector<Item> Game_State::items;
std::vector<Spell> Game_State::spells;

Item Game_State::plWeapon = Item();
Item Game_State::plArmor = Item();

int Game_State::selectedInv = SelectNone;
int Game_State::selectedSpell = SelectNone;
int Game_State::selectedSpecial = SelectNone;

bool Game_State::floorCopied = false;

std::vector<Item> Game_State::stockItem;
std::vector<Spell> Game_State::stockSpell;
std::vector<Special> Game_State::stockSpecial;
