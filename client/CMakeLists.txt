cmake_minimum_required(VERSION 4.0)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled2 main.cpp)

if (WIN32)
    target_link_libraries(untitled2 ws2_32)
endif()

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
