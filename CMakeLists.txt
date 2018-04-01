cmake_minimum_required(VERSION 3.9)
project(Projet)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestProjet Set.hpp test-set.cpp test-set.hpp)
add_executable(Projet main.cpp Set.hpp)