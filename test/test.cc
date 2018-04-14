#include <iostream>

#include "../tile.h"

using namespace topowave;

int main() {
    /* 1 2 3 7 7
     * 4 5 6 7 7
     * 7 8 9 7 7
     * 7 7 7 7 7
     * 7 7 7 7 7
     */
    std::vector<unsigned char> grid(1201 * 1201, 7);
    ElevationTile TestTile(grid);
    TestTile.elevation[0] = 1;
    TestTile.elevation[1] = 2;
    TestTile.elevation[2] = 3;
    TestTile.elevation[1201] = 4;
    TestTile.elevation[1202] = 5;
    TestTile.elevation[1203] = 6;
    TestTile.elevation[2402] = 7;
    TestTile.elevation[2403] = 8;
    TestTile.elevation[2404] = 9;

    PixelCoordinate test_px_coord(1, 1);

    auto pixel_data = TestTile.GetPixelData(test_px_coord);
    std::cout << pixel_data << std::endl;
}
