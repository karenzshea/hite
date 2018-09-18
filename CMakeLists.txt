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

add_library(catch2 STATIC
    test/catch.config.cpp
)

add_executable(main
   main.cpp
)

add_executable(tile-test
    test/tile.test.cpp
)

add_executable(tile-index-test
    test/tile_index.test.cpp
)

target_link_libraries(main hite)
target_link_libraries(tile-test catch2 hite)
target_link_libraries(tile-index-test catch2 hite)
