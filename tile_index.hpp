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
        bool IsValidTile(const int index);

        private:
        IntCoordinate parseCoordFromName(std::string fileName);
        int normalizeCoordToIndex(const IntCoordinate &coordinate);

        std::vector<ElevationTile> tiles{TILE_INDEX_SIZE};
    };
    void loadTileIndex(std::vector<const char*> &paths, std::vector<ElevationTile> &tiles);
    void readFileDir(const char* dirpath, std::vector<std::string> &files);
}
