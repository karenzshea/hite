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
    ElevationTile& GetTile(const int index);
    bool IsValidTile(const int index);

    private:
    IntCoordinate parseCoordFromName(std::string fileName);

    std::vector<ElevationTile> tiles{TILE_INDEX_SIZE};
};
int normalizeCoordToIndex(const IntCoordinate &coordinate);
void readFileDir(const char* dirpath, std::vector<std::string> &files);
}
