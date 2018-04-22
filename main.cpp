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

#include "tile.hpp"

int main(const int argc, const char* argv[]) {
    // Expecting file like 'N52E013.hgt'
    if (argc < 2) throw std::runtime_error("Missing argument file or path to files");
    const char* filepath = argv[1];
    struct stat filepath_stat;
    if (stat(filepath, &filepath_stat) == -1)
    {
        throw std::runtime_error("Could not stat file or file path");
    }
    std::vector<const char*> paths;
    // if filepath is a file push it into paths
    // if it's a directory, push all files *.hgt in the directory into paths
    if ((filepath_stat.st_mode & S_IFMT) == S_IFREG)
    {
        paths.push_back(std::move(filepath));
    }
    else if ((filepath_stat.st_mode & S_IFMT) == S_IFDIR)
    {
        DIR* dir_stream = dir_stream = opendir(filepath);
        struct dirent *dirp;
        if (dir_stream == NULL)
        {
            throw std::runtime_error("Could not open directory for reading");
        }
        std::regex re{".*+\\.hgt"};
        while ((dirp = readdir(dir_stream)) != NULL)
        {
            std::cout << std::string(dirp->d_name) << std::endl;
            if (std::regex_match(dirp->d_name, re))
            {
                paths.push_back(dirp->d_name);
            }
        }
    }
    std::cout << "Reading " << paths.size() << " path(s)..." << std::endl;

    // TODO read tile data into tile index structure for querying
    std::uint8_t i = 0;
    for (; i < paths.size(); i++)
    {
        char filename[strlen(filepath) + 1 + strlen(paths[i])];
        strcpy(filename, filepath);
        strcat(filename, "/");
        strcat(filename, paths[i]);
        std::cout << filename << std::endl;
        hite::ElevationTile tile(filename);

        hite::Coordinate top_of_hill(13.414049, 52.550679);
        hite::Elevation top = tile.GetElevation(top_of_hill);
        hite::Coordinate bottom_of_hill(13.409757, 52.528493);
        hite::Elevation bottom = tile.GetElevation(bottom_of_hill);
        std::cout << top << std::endl;
        std::cout << bottom << std::endl;
    }
}
