/**
*
* File: database_manager.h
* Description: Contain the declaration of the Database_Manager class, which represents a class that manages the game's database.
*
*/

#include <sqlite3.h>

#ifndef DATABASE_H
#define DATABASE_H

class Database_Manager {
private:
	/**
	* Constructor for Database_Manager.
	*/
	Database_Manager();

	static sqlite3* db;
	static unsigned int version;

public:

	/**
	* Method to load the database.
	*
	* Return:
	*	a bool. true if all assets are successfully loaded.
	*/
	static bool load();

	static bool migrate();

	static bool reverse(unsigned int target);

	static void close();

	static void resetDB();

	static void executeNonSelectStatement(const char* statement);

	static void executeSelect(const char* statement, int (*callback)(void*, int, char**, char**));
};


#endif