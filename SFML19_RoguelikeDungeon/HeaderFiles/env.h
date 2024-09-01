/**
*
* File: env.h
* Description: Env variables in C++ are surprising hard to retrieve.
* Define your env variables here instead.
*/

#include <array>

#ifndef ENV_H
#define ENV_H

// Production database version
constexpr unsigned int DATABASE_VERSION = 1;

// Array variables

constexpr std::array<long, 8> STARTING_STATS = { 0, 0, 0, 0, 0, 0, 0, 0 };

// If all variables below are false or 0, it means that the gameplay is intended for regular users.

constexpr bool ALWAYS_SPAWN_SHOP = true;
constexpr bool PLACE_SHOP_ON_PLAYER = true;
constexpr bool SKIP_SPAWN_ENEMY = false;
constexpr bool MODIFY_START_STATS = false;

constexpr int STARTING_INTERACTIBLES = 0;

constexpr unsigned int INTERACTIBLE_CHANCE = 0;
constexpr unsigned int STARTING_FLOOR = 0;
constexpr unsigned int STARTING_GOLD = 0;
constexpr unsigned int STARTING_STAT_PTS = 0;

#endif