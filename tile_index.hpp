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
        Elevation Lookup(const Coordinate &coordinate);

        private:
        Coordinate parseCoordFromName(const char* fileName);
        int normalizeCoordToIndex(const Coordinate &coordinate);

        std::array<ElevationTile, TILE_INDEX_SIZE> tiles;
    };
    void loadTileIndex(std::vector<const char*> &paths, std::vector<ElevationTile> &tiles);
    void readFileDir(const char* dirpath, std::vector<const char*> &files);
}
