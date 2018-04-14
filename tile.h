#ifndef topowave_TYPES_HPP
#define topowave_TYPES_HPP

#include <iostream>
#include <vector>
#include <cstdint>
#include <limits>

namespace topowave
{

using Elevation = std::int16_t;
constexpr const int MAX_TILE_SIZE = 1201;
constexpr const double MAX_DOUBLE = std::numeric_limits<double>::max();

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
    TileCoordinate(double &u, double &v)
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

struct ElevationTile {
    ElevationTile(int _x, int _y) : x(_x), y(_y), elevation(1201 * 1201) {
    }
    // TODO error handling if data read in is not of size 1201?
    ElevationTile(int _x, int _y, std::vector<unsigned char> &data) : x(_x), y(_y) {
        if (data.size() != MAX_TILE_SIZE)
        {
            throw std::runtime_error("Invalid hgt tile size!");
        }
    }
    ~ElevationTile() = default;

    // methods
    Elevation GetElevation(const Coordinate &coord);
    Elevation GetInterpolatedData(const TileCoordinate &tile_coord);
    TileCoordinate GetTileCoordinate(const Coordinate &coord);

    // members
    int x, y;
    std::vector<unsigned char> elevation;

    private:
    bool InsideTile(const Coordinate &coordinate);
    Elevation GetPixelData(const PixelCoordinate &pixel_coord);
};
}

#endif
