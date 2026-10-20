//
// Created by Student on 06.10.2026.
//

#pragma once

#include "sqlite3.h"
#include <iostream>

[[nodiscard]] int open_db();
[[nodiscard]] int create_table();
[[nodiscard]] bool checkAuth(const std::string& login, const std::string& password, std::string& response);