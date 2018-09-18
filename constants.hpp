#ifndef hite_CONSTANTS_HPP
#define hite_CONSTANTS_HPP

#include <limits>
#include <cstdint>

namespace hite
{
// TODO encode way to return no data for Elevation
using Elevation = std::int16_t;
constexpr const std::int16_t MAX_ELEVATION = std::numeric_limits<std::int16_t>::max();
constexpr const int MAX_INT = std::numeric_limits<int>::max();
constexpr const double MAX_DOUBLE = std::numeric_limits<double>::max();

constexpr const int MAX_TILE_SIZE = 1201;
constexpr const int TILE_INDEX_SIZE = 180 * 360;
constexpr const int HGT_FILE_SIZE = 25934402;
}
#endif
