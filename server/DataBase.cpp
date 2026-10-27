//
// Created by Student on 06.10.2026.
//

#include "DataBase.h"

#define DEBUG

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

bool checkAuth(const std::string &login, const std::string &password, std::string &response) {
    std::string sql = "SELECT PASSWORD FROM USERS WHERE LOGIN = ?";
    sqlite3_stmt *stmt;

    // формирование запроса в stmt
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
#ifdef DEBUG
        std::cout << "error while formating stmt " << std::string(sqlite3_errmsg(db));
#endif
        return false;
    }

    // подстановка логина вместо плейсхолдера (?), SQLITE_TRANSIENT - копирование значения во избежание его изменения за время ожидания
    result = sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_TRANSIENT);
    if (result != SQLITE_OK) {
#ifdef DEBUG
        std::cout << "error while binding text " << std::string(sqlite3_errmsg(db));
#endif
        return false;
    }

    // выполнение запроса
    result = sqlite3_step(stmt);

    // пользователь найден
    if (result == SQLITE_ROW) {
        auto dbPassword = std::string((char*)sqlite3_column_text(stmt, 0));

        if (dbPassword == password) {
            response = "Signed in";
        }
        else {
            response = "Invalid password";
        }

    }

    // регистрация
    else {
        std::string sqlRegistration = "INSERT INTO USERS (LOGIN, PASSWORD) VALUES (?, ?);";

        sqlite3_stmt *insertStmt;

        result = sqlite3_prepare_v2(db, sqlRegistration.c_str(), -1, &insertStmt, NULL);
        if (result != SQLITE_OK) {
#ifdef DEBUG
            std::cout << "error while formating insertStmt " << std::string(sqlite3_errmsg(db));
#endif
            return false;
        }

        result = sqlite3_bind_text(insertStmt, 1, login.c_str(), -1, SQLITE_TRANSIENT);
        result = sqlite3_bind_text(insertStmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_step(insertStmt);
        sqlite3_finalize(insertStmt);

        response = "You have been registered";
    }

    sqlite3_finalize(stmt);
    return true;
}
