/**
*
* File: sfml19.cpp
* Description: Has the main method to run the entire program.
*
*/

#include "interface.h"
#include "Manager/database_manager.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

int main() {
    try {
        bool success = Database_Manager::load();
        if (success) {
            std::srand(static_cast<unsigned int>(std::time(nullptr) * std::time(nullptr) / 2 + 3 * 5));
            srand(time(NULL));
            Interface::get().loopWindow();
        }

        // Database_Manager::reverse(0);
        Database_Manager::close();
    }
    catch (char* error) {
        printf(error);
        return -1;
    }
    return 0;
}