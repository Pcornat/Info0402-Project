cmake_minimum_required(VERSION 3.5.1)
project(Projet)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestProjet catch-unit.cpp Set.hpp test-set.cpp)
add_executable(Projet main.cpp Set.hpp)