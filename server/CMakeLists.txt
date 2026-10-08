cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp DataBase.cpp)

add_library(sqlite3 STATIC IMPORTED)
set_target_properties(sqlite3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/sqlite3.lib"
)

target_include_directories(untitled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SQLite)

if (WIN32)
    target_link_libraries(untitled PRIVATE sqlite3 ws2_32)
endif()

add_custom_command(TARGET untitled POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/SQLite/sqlite3.dll"
    $<TARGET_FILE_DIR:untitled>
)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
