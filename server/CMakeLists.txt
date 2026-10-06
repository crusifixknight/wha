cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp DataBase.cpp)

target_include_directories(untitled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SQLite)

if (WIN32)
    target_link_libraries(untitled ws2_32)
endif()

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
