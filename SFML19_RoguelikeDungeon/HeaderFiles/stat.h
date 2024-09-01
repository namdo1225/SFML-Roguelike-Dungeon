#ifndef STAT_H
#define STAT_H

/**
* Contains standardized stat names.
*/
class StatConst {
public:
    const static unsigned int NUM_STATS = 8;

    const static char* FULL_STATS[];
    const static char* ABR_STATS[];
};

/**
* Contains enum to standardize stat indices.
*/
enum Stat { Max_Hp, Max_Mp, Str, Mgk, Def, Res, Hp, Mp };

inline Stat& operator++(Stat& state, int) {
    const int i = static_cast<int>(state) + 1;
    state = static_cast<Stat>(i % StatConst::NUM_STATS);
    return state;
}

inline Stat& operator--(Stat& type, int) {
    const int i = static_cast<int>(type) - 1;
    type = static_cast<Stat>(i < 0 ? StatConst::NUM_STATS - 1 : i % StatConst::NUM_STATS);
    return type;
}

/**
* Contains enum to standardize attack type.
*/
enum Attack { Magical, Physical };

#endif