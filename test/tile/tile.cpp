#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "../../tile.hpp"

BOOST_AUTO_TEST_SUITE(tile_test)

using namespace hite;

BOOST_AUTO_TEST_CASE(elevation_tile_test)
{
    // NE
    Coordinate berlin_coord = {13.410047, 52.547868};
    // NW
    Coordinate detroit_coord = {-83.044281, 42.3534697};
    // SW
    Coordinate ayacucho_coord = {-74.222087, -13.167748};
    // SE
    Coordinate jakarta_coord = {106.842041, -6.206090};
    TileCoordinate berlin = GetTileCoordinate(berlin_coord);
    TileCoordinate berlin_expected = {.410047, .547868};
    BOOST_CHECK_EQUAL(berlin, berlin_expected);

    TileCoordinate detroit = GetTileCoordinate(detroit_coord);
    TileCoordinate detroit_expected = {.044281, .3534697};
    BOOST_CHECK_EQUAL(detroit, detroit_expected);

    TileCoordinate ayacucho = GetTileCoordinate(ayacucho_coord);
    TileCoordinate ayacucho_expected = {.222087, .167748};
    BOOST_CHECK_EQUAL(ayacucho, ayacucho_expected);

    TileCoordinate jakarta = GetTileCoordinate(jakarta_coord);
    TileCoordinate jakarta_expected = {.842041, .206090};
    BOOST_CHECK_EQUAL(jakarta, jakarta_expected);
}

BOOST_AUTO_TEST_SUITE_END()
