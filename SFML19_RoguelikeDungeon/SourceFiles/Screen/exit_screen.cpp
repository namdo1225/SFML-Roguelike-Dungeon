#include "Screen/exit_screen.h"
#include "interface.h"

Exit_Screen::Exit_Screen(sf::Font& font) : Screen(4, 4, font) {
	setup_helper(true, 0, NULL, 200, 200, 800, 400);
	setup_helper(true, 1, NULL, 260, 200, 200, 40);
	setup_helper(true, 2, NULL, 340, 200, 200, 40);
	setup_helper(true, 3, NULL, 420, 200, 200, 40);

	setup_helper(false, 0, "Any unsaved changes will be discarded. Do you still want to quit?", 205, 220, NULL, NULL);
	setup_helper(false, 1, "Cancel", 534, 343, NULL, NULL);
	setup_helper(false, 2, "Title Screen", 498, 423, NULL, NULL);
	setup_helper(false, 3, "Quit", 552, 503, NULL, NULL);
}

void Exit_Screen::event_handler(int x, int y) {
	if (mouse_in_helper(true, 1, x, y))
		visibilities[Exit] = false;
	else if (mouse_in_helper(true, 2, x, y)) {
		visibilities[Menu] = false;
		visibilities[Title] = true;
		Interface::get().reset_game();
	}
	else if (mouse_in_helper(true, 3, x, y))
		window.close();
}