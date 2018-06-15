#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
extern "C" {
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
};

#include "tile_index.hpp"
#include "tile.hpp"

int main(const int argc, const char* argv[]) {
    // Expecting file like 'N52E013.hgt'
    if (argc < 2) throw std::runtime_error("Missing argument file or path to files");
    const char* path = argv[1];
    struct stat path_stat;
    if (stat(path, &path_stat) == -1)
    {
        throw std::runtime_error("Could not stat file or file path");
    }

    // handle either a single file or a path to a directory
    if ((path_stat.st_mode & S_IFMT) == S_IFREG)
    {
        hite::TileIndex tile_index(path);
    }
    else if ((path_stat.st_mode & S_IFMT) == S_IFDIR)
    {
        std::vector<const char*> files;
        hite::readFileDir(path, files);
        hite::TileIndex tile_index(path, files);
    }
    /*

    hite::Coordinate top_of_hill(13.414049, 52.550679);
    hite::Elevation top = tile_index.Lookup(top_of_hill);
    hite::Coordinate bottom_of_hill(13.409757, 52.528493);
    hite::Elevation bottom = tile_index.Lookup(bottom_of_hill);
    std::cout << top << std::endl;
    std::cout << bottom << std::endl;
    */
}
