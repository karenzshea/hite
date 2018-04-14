#include <iostream>
#include <fstream>
#include <stdexcept>
#include <regex>
#include <string>

#include "tile.h"

int main(const int, const char* argv[]) {
    // Expecting file like 'N52E013.hgt'
    auto filepath = argv[1];
    std::regex re{"^.+([0-9]{2}).+([0-9]{3})\\.hgt"};
    std::cmatch out;
    int x, y;
    if (std::regex_match(filepath, out, re))
    {
        if (out.size() != 3) throw std::runtime_error("Could not parse file name");

        x = std::stoi(out[1]);
        y = std::stoi(out[2]);
    }
    else
    {
        throw std::runtime_error("Could not parse file name");
    }

    std::ifstream HgtFile;
    HgtFile.open(argv[1], std::ios::binary);
    if (!HgtFile.is_open())
    {
        throw std::runtime_error("Couldn't open hgt file!");
    }
    topowave::ElevationTile tile(x, y);
    auto beg = HgtFile.tellg();
    HgtFile.seekg(0, std::ios::end);
    auto end = HgtFile.tellg();
    // trust me these bytes will fit in tile.elevation
    HgtFile.read(reinterpret_cast<char *>(tile.elevation.data()), end-beg);
    HgtFile.close();

    topowave::Coordinate test_coord(13.4109, 52.5283);
    topowave::Elevation elevation = tile.GetElevation(test_coord);
    std::cout << elevation << std::endl;
}
