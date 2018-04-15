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

    hite::Coordinate test_coord1(13.4109, 52.5283);
    hite::Elevation elevation1 = tile.GetElevation(test_coord1);
    std::cout << elevation1 << std::endl;
}
