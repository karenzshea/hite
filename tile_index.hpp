#include <vector>
#include <array>
#include <string>

#include "tile.hpp"
#include "constants.hpp"

namespace hite
{
struct TileIndex {
    TileIndex();
    TileIndex(const std::vector<std::string> &files);
    Elevation Lookup(const Coordinate &coordinate);
    ElevationTile& getTile(const int index);

    private:
    Elevation calculateElevation(const Coordinate &coordinate);

    std::vector<ElevationTile> tiles{TILE_INDEX_SIZE};
};
int normalizeCoordToIndex(const IntCoordinate &coordinate);
void readFileDir(const char* dirpath, std::vector<std::string> &files);
IntCoordinate parseCoordFromName(std::string fileName);
}
