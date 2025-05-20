#ifndef B46CA402_5D14_4D1D_9923_49018BA7FA61
#define B46CA402_5D14_4D1D_9923_49018BA7FA61

#include <Position.hpp>
#include <string>

struct Cell {
    Position position;
    double size;
    std::string colorCode;

    Cell& operator=(const Cell&) = delete;  // コピー代入演算子を削除
    Cell& operator=(Cell&&) = delete;       // ムーブ代入演算子を削除
};

// enum class スコープを持つ列挙型で、intへ暗黙キャストされない。
enum class CellStatus { EMPTY, MOVING, FILLED };

#endif /* B46CA402_5D14_4D1D_9923_49018BA7FA61 */
