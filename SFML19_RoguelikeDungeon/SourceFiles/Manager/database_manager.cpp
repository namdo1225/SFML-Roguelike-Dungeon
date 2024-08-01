#include "Manager/database_manager.h"
#include <iostream>
#include <format>
#include "env.h"

sqlite3* Database_Manager::db;

unsigned int Database_Manager::version;

bool Database_Manager::load() {
    int rc = sqlite3_open("game.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    else {
        sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, "PRAGMA user_version;", -1, &stmt, NULL);

        if (rc != SQLITE_OK)
            throw sqlite3_errmsg(db);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            version = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);

        return migrate();
    }
}

bool Database_Manager::migrate() {
    if (version < DATABASE_VERSION) {
        switch (version) {
        case 0:
            executeNonSelectStatement("CREATE TABLE enemies ( \
                                  id INTEGER PRIMARY KEY, \
                                  name TEXT NOT NULL, \
                                  health INTEGER NOT NULL, \
                                  health_growth REAL DEFAULT 0.5, \
                                  attack INTEGER NOT NULL, \
                                  attack_growth REAL DEFAULT 0.5, \
                                  defense INTEGER NOT NULL, \
                                  defense_growth REAL DEFAULT 0.5, \
                                  resistance INTEGER NOT NULL, \
                                  resistance_growth REAL DEFAULT 0.5, \
                                  range INTEGER NOT NULL, \
                                  experience INTEGER NOT NULL, \
                                  experience_growth REAL DEFAULT 0.5, \
                                  attack_type INTEGER NOT NULL \
                                  floor INTEGER DEFAULT 1 \
            );");
            executeNonSelectStatement("INSERT INTO enemies VALUES(0, 'Zombie',   9, 0.5,  3,  0.5, 3,   0.5, 1, 0.25, 1,  3, 0.75,  1, 1);");
            executeNonSelectStatement("INSERT INTO enemies VALUES(1, 'Skeleton', 7, 0.4,  2, 0.25, 2,  0.25, 2,  0.5, 3,  3, 0.75,  1, 1);");
            executeNonSelectStatement("INSERT INTO enemies VALUES(2, 'Mage',     3, 0.3,  4,  0.5, 1, 0.125, 4,  0.5, 4,  3, 0.75,  0, 1);");
            executeNonSelectStatement("INSERT INTO enemies VALUES(3, 'Bandit',  12, 0.6,  4, 0.25, 5,  0.25, 3, 0.25, 2,  5, 1.50,  1, 1);");
            executeNonSelectStatement("INSERT INTO enemies VALUES(4, 'Mimic',    9, 0.7, 10,  0.7, 2,   0.4, 2,  0.4, 1, 10, 1, 1,  1, 5);");
            executeNonSelectStatement("INSERT INTO enemies VALUES(5, 'Warrior', 20, 0.7,  8,  0.5, 7,   0.5, 4,  0.4, 1,  8,  0.8,  1, 10);");
            executeNonSelectStatement("INSERT INTO enemies VALUES(6, 'Priest',   8, 0.3,  8,  0.5, 3, 0.125, 7,  0.5, 5,  9,  0.8,  0, 10);");
            version++;
        
            executeNonSelectStatement("CREATE TABLE spells ( \
                                  id INTEGER PRIMARY KEY, \
                                  name TEXT NOT NULL, \
                                  health INTEGER NOT NULL, \
                                  health_growth REAL DEFAULT 0.5, \
                                  attack INTEGER NOT NULL, \
                                  attack_growth REAL DEFAULT 0.5, \
                                  defense INTEGER NOT NULL, \
                                  defense_growth REAL DEFAULT 0.5, \
                                  resistance INTEGER NOT NULL, \
                                  resistance_growth REAL DEFAULT 0.5, \
                                  range INTEGER NOT NULL, \
                                  experience INTEGER NOT NULL, \
                                  experience_growth REAL DEFAULT 0.5, \
                                  attack_type INTEGER NOT NULL \
                                  floor INTEGER DEFAULT 1 \
            );");
        }

        executeNonSelectStatement(std::format("PRAGMA user_version = {};", version).c_str());
    }

	return true;
}

bool Database_Manager::reverse(unsigned int target) {
    if (version > target) {
        switch (version) {
        case 1:
            executeNonSelectStatement("DROP TABLE enemies;");
            version--;
        }

        executeNonSelectStatement(std::format("PRAGMA user_version = {};", version).c_str());
    }

    return true;
}

void Database_Manager::close() {
	sqlite3_close(db);
}

void Database_Manager::executeNonSelectStatement(const char* statement) {
    char* errmsg;
    if (sqlite3_exec(db, statement, 0, 0, &errmsg))
        throw errmsg;
}
