#ifndef B46CA402_5D14_4D1D_9923_49018BA7FA61
#define B46CA402_5D14_4D1D_9923_49018BA7FA61

#include <Position.hpp>
#include <string>

struct Cell {
    Position position;
    double size;
    std::string colorCode;
};

enum CellStatus { EMPTY, MOVING, FILLED };

#endif /* B46CA402_5D14_4D1D_9923_49018BA7FA61 */
