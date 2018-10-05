#include "../third_party/catch2/catch.hpp"
#include "../tile_index.hpp"

/*
TEST_CASE("Parse int coordinate from name", "[TileIndex]") {
    hite::IntCoordinate coord1 = hite::parseCoordFromName("./test/data/N28W016.hgt");
    hite::IntCoordinate coord2 = hite::parseCoordFromName("./test/data/N52E013.hgt");
    hite::IntCoordinate coord3 = hite::parseCoordFromName("./test/data/S12W077.hgt");
    hite::IntCoordinate coord4 = hite::parseCoordFromName("./test/data/S06E106.hgt");
    CHECK(coord1.IsValid());
    CHECK(coord2.IsValid());
    CHECK(coord3.IsValid());
    CHECK(coord4.IsValid());
    CHECK(coord1.Longitude == -16);
    CHECK(coord1.Latitude == 28);
    CHECK(coord2.Longitude == 13);
    CHECK(coord2.Latitude == 52);
    CHECK(coord3.Longitude == -77);
    CHECK(coord3.Latitude == -12);
    CHECK(coord4.Longitude == 106);
    CHECK(coord4.Latitude == -6);
}

TEST_CASE("Generate tile index", "[TileIndex]") {
    std::vector<std::string> files{"./test/data/N27W016.hgt", "./test/data/N28W016.hgt", "./test/data/N52E013.hgt"};
    hite::TileIndex index(files);
    // approximately mount teide on tenerife
    // https://www.answers.com/Q/How_high_is_Mount_Teide
    hite::Elevation pico = index.Lookup({-16.642575921786374,28.272692489729017});
    hite::Elevation xyz = index.Lookup({-16.66763305664062, 28.261751308162367});
    std::cout << "pico " << pico << std::endl;
    CHECK(pico >= 3710);
    CHECK(pico <= 3720);
}
*/
TEST_CASE("Generate tile index", "[TileIndex]") {
    std::vector<std::string> files{"./test/data/N45E007.hgt"};
    hite::TileIndex index(files);
    hite::Elevation xyz = index.Lookup({7.659044597979232,45.97652690664907});
    std::cout << "xyz " << xyz << std::endl;
    CHECK(xyz >= 4000);
}
