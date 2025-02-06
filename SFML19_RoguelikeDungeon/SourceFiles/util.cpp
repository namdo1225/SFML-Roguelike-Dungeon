/**
*
* File: util.cpp
* Description: Contains the implementation of the Util class.
*
*/

#include "util.h"
#include <cmath>
#include <Manager/sf_manager.h>
#include <string>
#include <vector>

std::vector<std::string> Util::split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

int Util::nearestMultiple(int value, int multiple) {
    // https://stackoverflow.com/questions/29557459/round-to-nearest-multiple-of-a-number
    int result = std::abs(value) + multiple / 2;
    result -= result % multiple;
    result *= value > 0 ? 1 : -1;

    return result;
}

int Util::nearestTile(int value) {
    return nearestMultiple(value, SF_Manager::TILE);
}
