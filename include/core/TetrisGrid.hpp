#ifndef EAEC85BA_F694_47C7_AB0E_2FD093BD0A16
#define EAEC85BA_F694_47C7_AB0E_2FD093BD0A16
#ifndef F7B71047_1F3E_482C_9849_3CB6B22E22C3
#define F7B71047_1F3E_482C_9849_3CB6B22E22C3

#include <algorithm>
#include <core/Cell.hpp>
#include <core/Position.hpp>
#include <core/Tetrimino.hpp>
#include <core/graphics_types.hpp>
#include <string>
#include <vector>

struct TetrisGrid {
    std::string id;
    Position position;  // グリッドの左上位置
    Size size;
    GridColumnRow grid_size;               // 行数と列数
    std::vector<std::vector<Cell>> cells;  // 各セルの状態を保持
};

namespace tetris_grid {
/**
 * TetrisGrid の生成関数
 * @param id グリッドの識別子
 * @param pos グリッドの左上位置
 * @param sz グリッドのサイズ
 * @param rows 行数
 * @param cols 列数
 * @return TetrisGrid インスタンス
 */
TetrisGrid create(const std::string& id, Position pos, Size sz, int rows, int cols) {
    return TetrisGrid{id, pos, sz, rows, cols};
}
}  // namespace tetris_grid

namespace tetris_grid_manipulation {

Position get_position_of_cell(const TetrisGrid& grid, const GridColumnRow& grid_position,
                              double cell_size);

GridColumnRow get_grid_position_of_cell(const TetrisGrid& grid, const Position& cell_position,
                                        double cell_size);

}  // namespace tetris_grid_manipulation

#endif /* F7B71047_1F3E_482C_9849_3CB6B22E22C3 */

#endif /* EAEC85BA_F694_47C7_AB0E_2FD093BD0A16 */
