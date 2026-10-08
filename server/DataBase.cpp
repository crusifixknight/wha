//
// Created by Student on 06.10.2026.
//

#include "DataBase.h"

void print() {
    std::cout << SQLITE_VERSION << std::endl;
}

sqlite3 *db;

int open_db() {
    int result = sqlite3_open("example.db", &db);
    return result;
}

int create_table() {
    char* errorMessage = nullptr;

    std::string table = "CREATE TABLE IF NOT EXISTS USERS("
                      "ID       INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "LOGIN    TEXT NOT NULL UNIQUE, "
                      "PASSWORD TEXT NOT NULL);";

    int result = sqlite3_exec(db, table.c_str(), NULL, NULL, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_free(errorMessage);
    }

    return result;
}
