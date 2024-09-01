#include "Shape/full_text.h"
#include <vector>

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

/**
* A class to manage the game's log.
*/
class Log_Manager {
public:
	const static unsigned int LOG_SIZE = 50;

	static std::vector<Full_Text> logs;

	/**
	* Change the position of logs based on the view.
	*
	* Parameter:
	*	screen: true if setting positions for the log screen.
	*		Otherwise, it's the game screen.
	*/
	static void log_view(bool screen = false);

	/**
	* Add a log.
	*
	* Parameter:
	*	log: Log's content.
	*/
	static void log_add(const char* log);
};

#endif