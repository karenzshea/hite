#include <cmath>
#include <string.h>

#include "./tile.hpp"

namespace hite
{
bool IntCoordinate::IsValid()
{
    return Longitude != MAX_INT && Latitude != MAX_INT;
}
bool operator==(const CoordinateDecimal& lhs, const CoordinateDecimal& rhs) { return std::tie(lhs.U, lhs.V) == std::tie(rhs.U, rhs.V); }

CoordinateDecimal GetCoordinateDecimal(const Coordinate &coord)
{
    CoordinateDecimal cd{std::abs(coord.Longitude), std::abs(coord.Latitude)};
    cd.U = cd.U - std::floor(cd.U);
    cd.V = cd.V - std::floor(cd.V);
    return cd;
}

// ElevationTile
Elevation ElevationTile::GetPixelData(const PixelCoordinate &pixel_coord)
{
    // bytes need to be swapped because of data format. From documentation:
    //    "The DEM is provided as 16-bit signed integer data in a simple binary
    //    raster. There are no header or trailer bytes embedded in the file. The data
    //    are stored in row major order (all the data for row 1, followed by all the
    //    data for row 2, etc.).
    //
    //    All elevations are in meters referenced to the WGS84 EGM96 geoid as
    //    documented at http://www.nima.mil/GandG/wgsegm/.
    //
    //    Byte order is Motorola ("big-endian") standard with the most significant
    //    byte first. Since they are signed integers elevations can range from -32767
    //    to 32767 meters, encompassing the range of elevation to be found on the
    //    Earth."
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
    CoordinateDecimal cd = GetCoordinateDecimal(coord);
    return cd.U == x && cd.V == y;
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
