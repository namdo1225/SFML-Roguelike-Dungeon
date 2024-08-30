/**
*
* File: stat.h
* Description: Contains a definition of Stat enum and StatConst class to standardize stat names.
*
*/

#ifndef STAT_H
#define STAT_H

class StatConst {
public:
    const static unsigned int NUM_STATS = 8;

    const static char* FULL_STATS[];
    const static char* ABR_STATS[];
};

enum Stat { Max_Hp, Max_Mp, Str, Mgk, Def, Res, Hp, Mp };

inline Stat& operator++(Stat& state, int) {
    const int i = static_cast<int>(state) + 1;
    state = static_cast<Stat>((i) % StatConst::NUM_STATS);
    return state;
}

inline Stat& operator--(Stat& type, int) {
    const int i = static_cast<int>(type) - 1;

    if (i < 0) {
        type = static_cast<Stat>(StatConst::NUM_STATS);
    }
    else {
        type = static_cast<Stat>((i) % StatConst::NUM_STATS);
    }
    return type;
}

enum Attack { Magical, Physical };

#endif