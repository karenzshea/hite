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
    IntCoordinate TileIndex::parseCoordFromName(std::string fileName)
    {
        // N52E013.hgt => {Lon: 13, Lat: 52}
        std::regex re{".?([NWSE]{1})([0-9]{2})([NWSE]{1})([0-9]{3})\\.hgt"};
        std::smatch out;
        IntCoordinate parsed;
        if (std::regex_match(fileName, out, re))
        {
            int y = out[1] == 'N' ? std::stoi(out[2]) : std::stoi(out[2]) * -1;
            int x = out[3] == 'E' ? std::stoi(out[4]) : std::stoi(out[4]) * -1;
            parsed.Longitude = x;
            parsed.Latitude = y;
        }
        else
        {
        }
        return parsed;
    }
    int TileIndex::normalizeCoordToIndex(const IntCoordinate &coordinate)
    {
        // {Lon: 13, Lat: 52} => 193 * 142 => 27406
        int offset_x = coordinate.Longitude + 180;
        int offset_y = coordinate.Latitude + 90;
        return offset_x * offset_y;
    }
    bool TileIndex::IsValidTile(const int index)
    {
        return (bool)tiles[index].mm.map;
    }
    Elevation TileIndex::Lookup(const Coordinate &coordinate)
    {
        //TileCoordinate tile_coord = GetTileCoordinate(coordinate);
        auto index = normalizeCoordToIndex(IntCoordinate{coordinate.Longitude, coordinate.Latitude});
        std::cout << "tile index: " << index << std::endl;
        if (IsValidTile(index))
        {
            return tiles[index].GetElevation(coordinate);
        }
        std::cout << "no valid tile found" << std::endl;
        return MAX_ELEVATION;
    }
    TileIndex::TileIndex() = default;
    TileIndex::TileIndex(const std::vector<std::string>& files)
    {
        std::cout << "Reading " << files.size() << " path(s)..." << std::endl;
        std::size_t i = 0;
        for (; i < files.size(); i++)
        {
            IntCoordinate file_coord = parseCoordFromName(files[i]);
            if (file_coord.Longitude == MAX_INT || file_coord.Latitude == MAX_INT)
            {
                std::cout << "Unable to parse file name: " << files[i] << std::endl;
                continue;
            }
            int coord_index = normalizeCoordToIndex(file_coord);
            tiles.at(coord_index) = ElevationTile(file_coord.Longitude, file_coord.Latitude, files[i]);
            std::cout << "wrote tile at: " << coord_index << std::endl;
        }
    }
    void readFileDir(const char* dirpath, std::vector<std::string>& files)
    {
        // save filenames of all .hgt files in a given directory
        std::string directory(dirpath);
        // TODO check that this is valid path
        DIR* dir_stream = dir_stream = opendir(dirpath);
        struct dirent *dirp;
        if (dir_stream == NULL)
        {
            throw std::runtime_error("Could not open directory for reading");
        }
        std::regex re{".*+\\.hgt"};
        while ((dirp = readdir(dir_stream)) != NULL)
        {
            if (std::regex_match(dirp->d_name, re))
            {
                files.push_back(directory + "/" + std::string(dirp->d_name));
            }
        }
    }
}
