#ifndef B6A2B782_FE61_4667_84AD_802250E4C851
#define B6A2B782_FE61_4667_84AD_802250E4C851

#include <Cell.hpp>
#include <Position.hpp>

struct TetrisGrid {
    Position position;
    double width;
    double height;
    int rows;
    int columns;
    std::vector<std::vector<Cell>> cells;
};

#endif /* B6A2B782_FE61_4667_84AD_802250E4C851 */
