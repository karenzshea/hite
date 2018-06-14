#include "../tile.hpp"

#include <iostream>

using namespace hite;

int main() {
    //Coordinate detroit = {-83.044281, 42.3534697};
    Coordinate berlin = {13.41585159, 52.4851846};
    auto tc = GetTileCoordinate(berlin);
    std::cout << tc.U << "," << tc.V << std::endl;

}
