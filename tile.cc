#include <cmath>

#include "./tile.h"

namespace topowave
{
Elevation ElevationTile::GetPixelData(const PixelCoordinate &pixel_coord)
{
    auto index = ((pixel_coord.Y) * MAX_TILE_SIZE) + pixel_coord.X;
    return static_cast<Elevation>(elevation[index]);
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
    int x1 = std::round(tile_coord.U * 1201);
    int y1 = 1201 - std::round(tile_coord.V * 1201);
    Elevation elevation = GetPixelData({x1, y1});
    return elevation;
}

TileCoordinate ElevationTile::GetTileCoordinate(const Coordinate &coord)
{
    TileCoordinate tc;
    tc.U = coord.Longitude - std::floor(coord.Longitude);
    tc.V = coord.Latitude - std::floor(coord.Latitude);
    return tc;
}

Elevation ElevationTile::GetElevation(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    Elevation elevation = GetInterpolatedData(tc);
    return elevation;
}
}
