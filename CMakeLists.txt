cmake_minimum_required(VERSION 3.4)
project(hite LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 14)

add_library(hite STATIC
   tile.cpp
   tile_index.cpp
)

add_executable(main
   main.cpp
)

target_link_libraries(main hite)
