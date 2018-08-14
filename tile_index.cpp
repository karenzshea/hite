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
    IntCoordinate parseCoordFromName(std::string fileName)
    {
        // N52E013.hgt => {Lon: 13, Lat: 52}
        std::regex re{".+\\/?([NWSE]{1})([0-9]{2})([NWSE]{1})([0-9]{3})\\.hgt"};
        std::smatch out;
        IntCoordinate parsed;
        if (std::regex_match(fileName, out, re))
        {
            int y = out[1] == 'N' ? std::stoi(out[2]) : std::stoi(out[2]) * -1;
            int x = out[3] == 'E' ? std::stoi(out[4]) : std::stoi(out[4]) * -1;
            parsed.Longitude = x;
            parsed.Latitude = y;
        }
        return parsed;
    }
    Elevation TileIndex::Lookup(const Coordinate &coordinate)
    {
        auto index = normalizeCoordToIndex(IntCoordinate{coordinate.Longitude, coordinate.Latitude});
        std::cout << "tile index: " << index << std::endl;
        if (tiles[index].isValid())
        {
            return calculateElevation(coordinate);
        }
        std::cout << "no valid tile found" << std::endl;
        return MAX_ELEVATION;
    }
    ElevationTile& TileIndex::getTile(const int index)
    {
        return tiles[index];
    }
    TileIndex::TileIndex() = default;
    TileIndex::TileIndex(const std::vector<std::string>& files)
    {
        std::cout << "Reading " << files.size() << " path(s)..." << std::endl;
        std::size_t i = 0;
        for (; i < files.size(); i++)
        {
            IntCoordinate file_coord = parseCoordFromName(files[i]);
            if (file_coord.IsValid())
            {
                std::cout << "Unable to parse file name: " << files[i] << std::endl;
                continue;
            }
            int coord_index = normalizeCoordToIndex(file_coord);
            tiles.at(coord_index) = ElevationTile(file_coord.Longitude, file_coord.Latitude, files[i]);
            std::cout << "wrote tile at: " << coord_index << std::endl;
        }
    }
    Elevation TileIndex::calculateElevation(const Coordinate &coordinate)
    {
        auto index = normalizeCoordToIndex(IntCoordinate{coordinate.Longitude, coordinate.Latitude});
        ElevationTile& tile = getTile(index);
        CoordinateDecimal coord_decimal = GetCoordinateDecimal(coordinate);
        Elevation q11, q21, q12, q22;
        // TODO edge cases when x2/y2 are > max_tile_size?
        int x1 = std::floor(coord_decimal.U * MAX_TILE_SIZE);
        int y1 = MAX_TILE_SIZE - std::floor(coord_decimal.V * MAX_TILE_SIZE);
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        q12 = tile.GetPixelData({x1, y2});
        q22 = tile.GetPixelData({x2, y2});
        q11 = tile.GetPixelData({x1, y1});
        q21 = tile.GetPixelData({x2, y1});
        double normalizedU = coord_decimal.U * MAX_TILE_SIZE;
        double normalizedV = MAX_TILE_SIZE - (coord_decimal.V * MAX_TILE_SIZE);

        double r1 = (((double)x2 - normalizedU) * q11) + ((normalizedU - (double)x1) * q21);
        double r2 = (((double)x2 - normalizedU) * q12) + ((normalizedU - (double)x1) * q22);
        double elevation = (((double)y2 - normalizedV)) * r1 + ((normalizedV - (double)y1) * r2);
        return elevation;
    }
    int normalizeCoordToIndex(const IntCoordinate &coordinate)
    {
        // e.g. {Lon: 13, Lat: 52} => 193 * 142 => 27406
        int offset_x = coordinate.Longitude + 180;
        int offset_y = coordinate.Latitude + 90;
        return offset_x * offset_y;
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
