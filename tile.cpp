#include <cmath>
#include <string.h>

#include "./tile.hpp"

namespace hite
{
// TODO this needs a better name
TileCoordinate GetTileCoordinate(const Coordinate &coord)
{
    TileCoordinate tc{std::abs(coord.Longitude), std::abs(coord.Latitude)};
    tc.U = tc.U - std::floor(tc.U);
    tc.V = tc.V - std::floor(tc.V);
    return tc;
}

// ElevationTile
Elevation ElevationTile::GetPixelData(const PixelCoordinate &pixel_coord)
{
    auto lower_idx = 2 * ((pixel_coord.Y * MAX_TILE_SIZE) + pixel_coord.X);
    auto upper_idx = lower_idx + 1;
    if (mm.map)
    {
        int16_t swapped = (static_cast<int16_t>(mm.map[lower_idx]) << 8) | static_cast<int16_t>(mm.map[upper_idx]);
        return swapped;
    }
    return MAX_ELEVATION;
}

Elevation ElevationTile::GetInterpolatedData(const Coordinate &coord)
{
    TileCoordinate coord_decimal = GetTileCoordinate(coord);
    Elevation q11, q21, q12, q22;
    int x1 = std::floor(coord_decimal.U * MAX_TILE_SIZE); // this can never be more than max tile size?
    int x2 = x1 + 1;
    int y1 = MAX_TILE_SIZE - std::floor(coord_decimal.V * MAX_TILE_SIZE);
    int y2 = y1 + 1;

    q12 = GetPixelData({x1, y2});
    q22 = GetPixelData({x2, y2});
    q11 = GetPixelData({x1, y1});
    q21 = GetPixelData({x2, y1});
    double normalizedU = coord_decimal.U * MAX_TILE_SIZE;
    double normalizedV = MAX_TILE_SIZE - (coord_decimal.V * MAX_TILE_SIZE);

    double r1 = (((double)x2 - normalizedU) * q11) + ((normalizedU - (double)x1) * q21);
    double r2 = (((double)x2 - normalizedU) * q12) + ((normalizedU - (double)x1) * q22);
    double elevation = (((double)y2 - normalizedV)) * r1 + ((normalizedV - (double)y1) * r2);
    return elevation;
}

bool ElevationTile::InsideTile(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    return tc.U == x && tc.V == y;
}

Elevation ElevationTile::GetElevation(const Coordinate &coord)
{
    Elevation elevation = GetInterpolatedData(coord);
    return elevation;
}

ElevationTile::ElevationTile(int _x, int _y, const std::string &filepath)
    : x(_x), y(_y), mm(filepath) {}
ElevationTile::ElevationTile() = default;
ElevationTile::~ElevationTile() = default;
}
