//
// Created by Student on 06.10.2026.
//

#include "DataBase.h"

void print() {
    std::cout << SQLITE_VERSION << std::endl;
}

int db_build() {
    sqlite3 *db;
    char* errorMessage = nullptr;

    int exit = sqlite3_open("example.db", &db);
    if (exit != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return exit;
    }
    std::cout << "Database opened" << std::endl;

    std::string sql = "CREATE TABLE IF NOT EXISTS USERS("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "NAME TEXT NOT NULL, "
                      "AGE INT NOT NULL, "
                      "EMAIL TEXT UNIQUE);";

    exit = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << "Table created" << std::endl;
    }

    sqlite3_close(db);

    return SQLITE_OK;
}