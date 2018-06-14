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
    // Q should this use an int based Coordinate type?
    Coordinate TileIndex::parseCoordFromName(const char* fileName)
    {
        std::regex re{"^([NWSE]{1})([0-9]{2})([NWSE]{1})([0-9]{3})\\.hgt"};
        std::cmatch out;
        int x,y;
        Coordinate parsed;
        if (std::regex_match(fileName, out, re))
        {
            x = out[1] == 'N' ? std::stoi(out[2]) : std::stoi(out[2]) * -1;
            y = out[3] == 'W' ? std::stoi(out[4]) : std::stoi(out[4]) * -1;
            parsed.Longitude = x;
            parsed.Latitude = y;
        }
        return parsed;
    }
    int TileIndex::normalizeCoordToIndex(const Coordinate &coordinate)
    {
        int offset_x = coordinate.Longitude + 180;
        int offset_y = coordinate.Latitude + 90;
        return offset_x * offset_y;
    }
    bool TileIndex::IsValidTile(const int index)
    {
        return tiles[index].map != nullptr;
    }
    Elevation TileIndex::Lookup(const Coordinate &coordinate)
    {
        // identify tile index of coordinate
        // check that tile at tile index is valid
        // call getElevation on tile
        auto index = normalizeCoordToIndex(coordinate);
        if (IsValidTile(index))
        {
            return tiles[index].GetElevation(coordinate);
        }
        return MAX_ELEVATION;
    }
    TileIndex::TileIndex(const char* dirpath, const std::vector<const char*> &files)
    {
        std::uint8_t x,y;
        std::size_t i = 0;
        for (; i < files.size(); i++)
        {
            std::regex re{"^.+([0-9]{2}).+([0-9]{3})\\.hgt"};
            std::cmatch out;
            if (std::regex_match(files[i], out, re))
            {
                if (out.size() != 3) throw std::runtime_error("Could not parse file name");

                x = std::stoi(out[1]);
                y = std::stoi(out[2]);
            }
            else
            {
                throw std::runtime_error("Could not parse file name");
            }
            // full path of file
            char fullpath[strlen(dirpath) + 1 + strlen(files[i])];
            strcpy(fullpath, dirpath);
            strcat(fullpath, "/");
            strcat(fullpath, files[i]);

            Coordinate file_coord = parseCoordFromName(files[i]);
            int coord_index = normalizeCoordToIndex(file_coord);
            tiles[coord_index] = ElevationTile(x, y, fullpath);
        }
    }
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
