#include <cmath>

#include "./tile.hpp"

namespace hite
{
TileCoordinate GetTileCoordinate(const Coordinate &coord)
{
    TileCoordinate tc{std::abs(coord.Longitude), std::abs(coord.Latitude)};
    tc.U = tc.U - std::floor(tc.U);
    tc.V = tc.V - std::floor(tc.V);
    return tc;
}

Elevation ElevationTile::GetPixelData(const PixelCoordinate &pixel_coord)
{
    auto lower_idx = 2 * ((pixel_coord.Y * MAX_TILE_SIZE) + pixel_coord.X);
    auto upper_idx = lower_idx + 1;
    int16_t swapped = static_cast<int16_t>(map[lower_idx]) << 8 | static_cast<int16_t>(map[upper_idx]);
    return swapped;
}

bool ElevationTile::InsideTile(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    return tc.U == x && tc.V == y;
}

Elevation ElevationTile::GetInterpolatedData(const TileCoordinate &tile_coord)
{
    // TODO bilinear interpolation?
    // convert tile coordinate into pixel coordinate
    int x1 = std::round(tile_coord.U * MAX_TILE_SIZE);
    int y1 = MAX_TILE_SIZE - std::round(tile_coord.V * MAX_TILE_SIZE);
    Elevation elevation = GetPixelData({x1, y1});
    return elevation;
}

Elevation ElevationTile::GetElevation(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    Elevation elevation = GetInterpolatedData(tc);
    return elevation;
}
}