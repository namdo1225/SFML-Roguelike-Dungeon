/**
*
* File: stat.h
* Description: Contains a definition of Stat enum to make it easier to read and write to the player's stat.
*
*/

#ifndef STAT_H
#define STAT_H

enum Stat { Max_Hp, Max_Mp, Str, Mgk, Def, Res, Hp, Mp };
const unsigned int NUM_STATS = 8;

inline Stat& operator++(Stat& state, int) {
    const int i = static_cast<int>(state) + 1;
    state = static_cast<Stat>((i) % NUM_STATS);
    return state;
}

inline Stat& operator--(Stat& type, int) {
    const int i = static_cast<int>(type) - 1;

    if (i < 0) {
        type = static_cast<Stat>(NUM_STATS);
    }
    else {
        type = static_cast<Stat>((i) % NUM_STATS);
    }
    return type;
}

#endif