#ifndef hite_TILE_HPP
#define hite_TILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <regex>
#include <string>
extern "C" {
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
};

namespace hite
{

using Elevation = std::int16_t;
constexpr const int MAX_TILE_SIZE = 3601;
constexpr const double MAX_DOUBLE = std::numeric_limits<double>::max();
constexpr const int NUM_DEGREE_TILES = 360 * 180;

struct Coordinate {
    Coordinate(double Lon, double Lat)
        : Longitude(Lon), Latitude(Lat) {
    }
    double Longitude;
    double Latitude;
};

struct TileCoordinate {
    TileCoordinate() : U(MAX_DOUBLE), V(MAX_DOUBLE)
    {
    }
    TileCoordinate(double u, double v)
        : U(u), V(v) {
    }
    double U; // column
    double V; // row
};

struct PixelCoordinate {
    PixelCoordinate(int x, int y)
        : X(x), Y(y) {
    }
    int X; // column
    int Y; // row
};

TileCoordinate GetTileCoordinate(const Coordinate &coord);

struct ElevationTile {
    ElevationTile(int _x, int _y, const char* filepath) : x(_x), y(_y) {
        // mmap tile file
        fd = open(filepath, O_RDWR, (mode_t)0222);
        if (fd == -1)
        {
            throw std::runtime_error("Couldn't open hgt file!");
        }
        file_stat = {0};
        if (fstat(fd, &file_stat) == -1)
        {
            throw std::runtime_error("Couldn't get size of hgt file!");
        }
        if (file_stat.st_size == 0 || file_stat.st_size != 25934402)
        {
            throw std::runtime_error("Hgt file is empty or the wrong size!");
        }
        map = static_cast<char *>(mmap(0, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
        if (map == MAP_FAILED)
        {
            throw std::runtime_error("Error mmapping file!");
        }
    }
    ElevationTile(const char* filepath) {
        std::regex re{"^.+([0-9]{2}).+([0-9]{3})\\.hgt"};
        std::cmatch out;
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
        ElevationTile(x, y, filepath);
    }
    ElevationTile(int _x, int _y) : x(_x), y(_y),
        elevation((MAX_TILE_SIZE * MAX_TILE_SIZE) * sizeof(int16_t)) {
    }
    ~ElevationTile() {
        if (munmap(map, file_stat.st_size) == -1)
        {
            close(fd);
        }
        close(fd);
    };

    // methods
    Elevation GetElevation(const Coordinate &coord);
    Elevation GetInterpolatedData(const TileCoordinate &tile_coord);
    Elevation GetPixelData(const PixelCoordinate &pixel_coord);

    // members
    int x, y;
    std::vector<unsigned char> elevation;
    char *map;
    struct stat file_stat;

    private:
    bool InsideTile(const Coordinate &coordinate);
    int fd;
};
}

#endif
