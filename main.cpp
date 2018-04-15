#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "tile.hpp"

int main(const int argc, const char* argv[]) {
    // Expecting file like 'N52E013.hgt'
    if (argc < 2) throw std::runtime_error("Missing argument hgt file");
    auto filepath = argv[1];

    hite::ElevationTile tile(filepath);

    hite::Coordinate top_of_hill(13.414049, 52.550679);
    hite::Elevation top = tile.GetElevation(top_of_hill);
    hite::Coordinate bottom_of_hill(13.409757, 52.528493);
    hite::Elevation bottom = tile.GetElevation(bottom_of_hill);
    std::cout << top << std::endl;
    std::cout << bottom << std::endl;
}
