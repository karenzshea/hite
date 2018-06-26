cmake_minimum_required(VERSION 3.4)
project(hite LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 14)
message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS "-Wall")

add_library(hite STATIC
   tile.cpp
   tile_index.cpp
   mem_mapper.cpp
)

add_executable(main
   main.cpp
)

target_link_libraries(main hite)
