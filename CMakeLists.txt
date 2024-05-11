cmake_minimum_required(VERSION 3.10.1)

project(Poker)

#set(SOURCES main.cpp deck.cpp)

include_directories(/usr/include/eigen3)

add_library(Deck deck.cpp)
add_library(Player player.cpp)
add_library(Game game.cpp)
add_library(Strat strat.cpp)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(Player Strat)
target_link_libraries(Poker Deck)
target_link_libraries(Poker Player)
target_link_libraries(Poker Game)

#To run: cmake -S ./ -B ./build -DCMAKE_BUILD_TYPE=Debug
