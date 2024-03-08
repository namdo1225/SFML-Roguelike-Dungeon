#ifndef EXIT_SCREEN_H
#define EXIT_SCREEN_H

#include "screen.h"

class Exit_Screen : protected Screen {
private:

public:
	Exit_Screen(sf::Font& font);

	void event_handler(int x, int y);
};

#endif