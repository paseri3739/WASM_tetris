#include <core/TetrisGrid.hpp>

// セルの座標を算出
Position TetrisGrid::get_position_of_cell(const GridColumnRow& grid_position, double cell_size) {
    return Position{
        this->position.x + grid_position.column * cell_size,
        this->position.y + grid_position.row * cell_size,
    };
}

// 座標からグリッド上の行・列を逆算（浮動小数をintに切り下げ）
GridColumnRow TetrisGrid::get_grid_position_of_cell(const Position& cell_position,
                                                    double cell_size) {
    int col = static_cast<int>((cell_position.x - this->position.x) / cell_size);
    int row = static_cast<int>((cell_position.y - this->position.y) / cell_size);
    return GridColumnRow{col, row};
}

// 範囲内チェック（整数インデックス）
bool TetrisGrid::is_within_bounds(int column, int row) {
    return column >= 0 && column < this->grid_size.column && row >= 0 && row < this->grid_size.row;
}

// 範囲内チェック（座標位置）
bool TetrisGrid::is_within_bounds(const Position& position) {
    return position.x >= this->position.x && position.y >= this->position.y &&
           position.x < this->position.x + this->size.width &&
           position.y < this->position.y + this->size.height;
}

// セルがFILLED状態か確認
bool TetrisGrid::is_filled_cell(const GridColumnRow& grid_position) const {
    int col = grid_position.column;
    int row = grid_position.row;
    if (!((col >= 0 && col < this->grid_size.column) && (row >= 0 && row < this->grid_size.row))) {
        return false;
    }
    return this->cells[row][col].type == CellStatus::FILLED;
}
