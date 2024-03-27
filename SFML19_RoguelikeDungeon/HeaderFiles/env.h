/**
*
* File: env.h
* Description: Env variables in C++ are surprising hard to retrieve.
* Define your env variables here instead.
*/

#ifndef ENV_H
#define ENV_H

// If all variables are false, it means that the gameplay is intended for regular users.

constexpr auto ALWAYS_SPAWN_STAIR = true;
constexpr auto SKIP_SPAWN_ENEMY = true;

// Start the game with 1000 golds.
constexpr auto START_WITH_MONEY = true;

#endif