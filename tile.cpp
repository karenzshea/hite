#include <cmath>
#include <string.h>

#include "./tile.hpp"

namespace hite
{
bool IntCoordinate::IsValid()
{
    return Longitude != MAX_INT && Latitude != MAX_INT;
}
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

bool ElevationTile::InsideTile(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    return tc.U == x && tc.V == y;
}

bool ElevationTile::isValid()
{
    return (bool)mm.map;
}

ElevationTile::ElevationTile(int _x, int _y, const std::string &filepath)
    : x(_x), y(_y), mm(filepath) {}
ElevationTile::ElevationTile() = default;
ElevationTile::~ElevationTile() = default;
}
