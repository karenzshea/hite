#include "../third_party/catch2/catch.hpp"
#include "../tile.hpp"

#include <iostream>

TEST_CASE("coordinate to tile coordinate", "[ElevationTile]") {
    const hite::CoordinateDecimal expected{.20826721191406,.87127072947452};
    const hite::Coordinate input{151.20826721191406,-33.87127072947452};
    const auto gotten = GetCoordinateDecimal(input);
    CHECK(gotten.V == Approx(expected.V));
    CHECK(gotten.U == Approx(expected.U));
}

TEST_CASE("elevation tile construction", "[ElevationTile]") {
    hite::ElevationTile et(52, 13, "./test/data/N52E013.hgt");
    CHECK(et.isValid());

    hite::ElevationTile et2 = std::move(et);
    CHECK(et2.isValid());
    CHECK_FALSE(et.isValid());

    hite::ElevationTile et3(std::move(et2));
    CHECK(et3.isValid());
    CHECK_FALSE(et2.isValid());
}

TEST_CASE("elevation tile pixel data", "[ElevationTile]") {
    hite::ElevationTile tile(52, 13, "./test/data/N52E013.hgt");
    // essentially a query for (13.5, 52.5)
    int x1 = std::floor(0.5 * hite::MAX_TILE_SIZE);
    int y1 = std::floor(0.5 * hite::MAX_TILE_SIZE);
    hite::Elevation elevation1 = tile.GetPixelData({x1, y1});
    int x2 = std::floor(0.25 * hite::MAX_TILE_SIZE);
    int y2 = std::floor(0.25 * hite::MAX_TILE_SIZE);
    hite::Elevation elevation2 = tile.GetPixelData({x2, y2});
    int x3 = std::floor(0.75 * hite::MAX_TILE_SIZE);
    int y3 = std::floor(0.75 * hite::MAX_TILE_SIZE);
    hite::Elevation elevation3 = tile.GetPixelData({x3, y3});
    CHECK(elevation1 == 35);
    CHECK(elevation2 == 33);
    CHECK(elevation3 == 32);
}
