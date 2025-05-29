#include <core/TetrisGrid.hpp>

Position tetris_grid::get_position_of_cell(const TetrisGrid& grid,
                                           const GridColumnRow& grid_position, double cell_size) {
    // グリッドの左上位置に、セルの位置を加算してセルの位置を計算
    return Position{grid.position.x + grid_position.column * cell_size,
                    grid.position.y + grid_position.row * cell_size};
}

GridColumnRow tetris_grid::get_grid_position_of_cell(const TetrisGrid& grid,
                                                     const Position& cell_position,
                                                     double cell_size) {
    // グリッドの左上位置からセルの位置を引いて、セルの列と行を計算
    int column = static_cast<int>((cell_position.x - grid.position.x) / cell_size);
    int row = static_cast<int>((cell_position.y - grid.position.y) / cell_size);
    return GridColumnRow{column, row};
}

bool tetris_grid::is_within_bounds(const TetrisGrid& grid, int column, int row) {
    bool isColumnValid = column >= 0 && column <= grid.grid_size.column;
    bool isRowValid = row >= 0 && row <= grid.grid_size.row;
    return isColumnValid && isRowValid;
}
