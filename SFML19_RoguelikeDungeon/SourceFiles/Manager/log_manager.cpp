/**
*
* File: log_manager.cpp
* Description: Contains the implementation of the Log_Manager class.
*
*/

#include "Manager/log_manager.h"
#include <Shape/full_text.h>
#include <vector>

const unsigned int Log_Manager::LOG_SIZE;

std::vector<Full_Text> Log_Manager::logs;

void Log_Manager::log_add(const char* log) {
    unsigned int size = logs.size();
    if (size == LOG_SIZE) {
        logs.erase(logs.begin());
        for (unsigned int i = 0; i < size; i++) {
            logs[i].setPosition(755, logs[i].getPosition().y - 30);
        }
    }
    logs.push_back(Full_Text(755.f, 465 + size * 30, 20.f, log, false));
    logs.back().setThemeAndHover(false, true);

    bool less_eight = logs.size() < 8;
    for (unsigned int i = less_eight ? 0 : logs.size() - 8, offset = 0; i < logs.size(); i++, offset++)
        logs[i].setPosition(755.f, 500.f + 30.f * offset);
}

void Log_Manager::log_view(bool screen) {
    if (screen) {
        for (unsigned int i = 0; i < logs.size(); i++)
            logs[i].setPosition(60.f, 120.f + 30.f * i);
    }
    else {
        bool less_eight = logs.size() < 8;
        for (unsigned int i = less_eight ? 0 : logs.size() - 8, offset = 0; i < logs.size(); i++, offset++)
            logs[i].setPosition(755.f, 500.f + 30.f * offset);
    }
}