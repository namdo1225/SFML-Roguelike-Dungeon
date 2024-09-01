#include <sqlite3.h>

#ifndef DATABASE_H
#define DATABASE_H

/**
* A class that manages the game's database.
*/
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

	/**
	* Migrates database to a higher version.
	* 
	* Return:
	*	true if migration is successful.
	*/
	static bool migrate();

	/**
	* Reverses database to a target version.
	*
	* Parameter:
	*	target: target database version.
	* 
	* Return:
	*	true if migration is successful.
	*/
	static bool reverse(unsigned int target);

	/**
	* Closes database.
	*/
	static void close();

	/**
	* Resets database.
	*/
	static void resetDB();

	/**
	* Executes a non-SELECT SQL statement.
	*
	* Parameter:
	*	statement: the statement to run.
	*/
	static void executeNonSelectStatement(const char* statement);

	/**
	* Executes a SELECT SQL statement.
	*
	* Parameter:
	*	statement: the statement to run.
	*	callback: the callback function when a row is retrieved.
	*/
	static void executeSelect(const char* statement, int (*callback)(void*, int, char**, char**));
};


#endif