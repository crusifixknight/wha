cmake_minimum_required(VERSION 4.0)
project(client)

set(CMAKE_CXX_STANDARD 20)

add_executable(client main.cpp)

if (WIN32)
    target_link_libraries(client ws2_32)
endif()

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
