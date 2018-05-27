#include <vector>
#include "tile.hpp"

namespace hite
{
    constexpr const int TILE_INDEX_SIZE = 180 * 360;
    struct TileIndex {
        TileIndex(const char* dirpath, std::vector<const char*> &files)
        {
            std::uint8_t i = 0;
            std::uint8_t x,y;
            for (; i < files.size(); i++)
            {
                std::regex re{"^.+([0-9]{2}).+([0-9]{3})\\.hgt"};
                std::cmatch out;
                if (std::regex_match(files[i], out, re))
                {
                    if (out.size() != 3) throw std::runtime_error("Could not parse file name");

                    x = std::stoi(out[1]);
                    y = std::stoi(out[2]);
                }
                else
                {
                    throw std::runtime_error("Could not parse file name");
                }
                // full path of file
                char fullpath[strlen(dirpath) + 1 + strlen(files[i])];
                strcpy(fullpath, dirpath);
                strcat(fullpath, "/");
                strcat(fullpath, files[i]);

                Coordinate file_coord = parseCoordFromName(files[i]);
                int coord_index = normalizeCoordToIndex(file_coord);
                tiles[coord_index] = ElevationTile(x, y, fullpath);
            }
        }
        ElevationTile* lookup(Coordinate &coordinate);

        private:
        Coordinate parseCoordFromName(const char* fileName);
        int normalizeCoordToIndex(Coordinate coordinate);
        std::vector<ElevationTile> tiles{TILE_INDEX_SIZE};
    };
    void loadTileIndex(std::vector<const char*> &paths, std::vector<ElevationTile> &tiles);
    void readFileDir(const char* dirpath, std::vector<const char*> &files);
}
