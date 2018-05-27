#include "tile_index.hpp"
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

namespace hite
{
    void readFileDir(const char* dirpath, std::vector<const char*> &files)
    {
        // save filenames of all .hgt files in a given directory
        DIR* dir_stream = dir_stream = opendir(dirpath);
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
                files.push_back(dirp->d_name);
            }
        }
    }
}
