#include <vector>
#include <array>
#include "tile.hpp"
#include "constants.hpp"

namespace hite
{
    struct TileIndex {
        // Q - does default dtor call dtor of ElevationTiles in `tiles` ?
        TileIndex();
        TileIndex(const char* dirpath, const std::vector<const char*> &files);
        TileIndex(const char* file);
        Elevation Lookup(const Coordinate &coordinate);
        bool IsValidTile(const int index);

        private:
        Coordinate parseCoordFromName(const char* fileName);
        int normalizeCoordToIndex(const Coordinate &coordinate);

        std::vector<ElevationTile> tiles{TILE_INDEX_SIZE};
    };
    void loadTileIndex(std::vector<const char*> &paths, std::vector<ElevationTile> &tiles);
    void readFileDir(const char* dirpath, std::vector<const char*> &files);
}
