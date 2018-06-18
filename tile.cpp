#include <cmath>
#include <string.h>

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

// ElevationTile
Elevation ElevationTile::GetPixelData(const PixelCoordinate &pixel_coord)
{
    auto lower_idx = 2 * ((pixel_coord.Y * MAX_TILE_SIZE) + pixel_coord.X);
    auto upper_idx = lower_idx + 1;
    // TODO look into operator precedence
    int16_t swapped = static_cast<int16_t>(map[lower_idx]) << 8 | static_cast<int16_t>(map[upper_idx]);
    return swapped;
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

bool ElevationTile::InsideTile(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    return tc.U == x && tc.V == y;
}

Elevation ElevationTile::GetElevation(const Coordinate &coord)
{
    TileCoordinate tc = GetTileCoordinate(coord);
    Elevation elevation = GetInterpolatedData(tc);
    return elevation;
}

ElevationTile::ElevationTile(int _x, int _y, const std::string &filepath) : x(_x), y(_y) {
    // mmap tile file
    fd = open(filepath.c_str(), O_RDWR, (mode_t)0222);
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
ElevationTile::ElevationTile() = default;
ElevationTile::~ElevationTile() {
    if (map == nullptr) return;
    if (munmap(map, file_stat.st_size) == -1)
    {
        std::cerr << "munmap failed with " << ::strerror(errno) << std::endl;
    }
    ::close(fd);
};
}
