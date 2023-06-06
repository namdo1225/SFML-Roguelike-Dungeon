/**
*
* File: sfml19.cpp
* Description: Has the main method to run the entire program.
*
*/

#include "interface_class.h"

int main() {

    std::srand(static_cast<unsigned int>(std::time(nullptr) * std::time(nullptr) / 2 + 3 * 5));
    srand(time(NULL));

    Interface::get().window_loop();

    return 0;
}