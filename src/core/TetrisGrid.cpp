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

bool TetrisGrid::is_colliding(const GridColumnRow& before, const GridColumnRow& after) const {
    // まず両方の位置がグリッド内に収まっているか確認
    bool before_in_bounds = before.column >= 0 && before.column < grid_size.column &&
                            before.row >= 0 && before.row < grid_size.row;
    bool after_in_bounds = after.column >= 0 && after.column < grid_size.column && after.row >= 0 &&
                           after.row < grid_size.row;

    if (!(before_in_bounds && after_in_bounds)) {
        return true;  // 領域外への移動は衝突とみなす
    }

    // セルの状態を取得
    const CellStatus from_status = cells[before.row][before.column].type;
    const CellStatus to_status = cells[after.row][after.column].type;

    // EMPTY から EMPTY への移動だけが衝突しない
    if (from_status == CellStatus::EMPTY && to_status == CellStatus::EMPTY) {
        return false;  // 非衝突
    }

    return true;  // それ以外は衝突扱い
}

TetrisGrid TetrisGrid::update_cell(const GridColumnRow& pos, CellStatus status, Color color) const {
    if (pos.row < 0 || pos.row >= this->grid_size.row || pos.column < 0 ||
        pos.column >= this->grid_size.column) {
        return *this;  // 範囲外 → 変更なし
    }

    const Cell& old_cell = this->cells[pos.row][pos.column];
    if (!is_legal_transition(old_cell.type, status)) {
        return *this;  // 不正遷移 → 変更なし
    }

    if (status == CellStatus::EMPTY) {
        color = Color::from_string("white");
    }

    auto updated_cell_result = CellFactory::update_cell_state(old_cell, status, color);
    if (!updated_cell_result.has_value()) {
        return *this;  // 失敗 → 元のまま
    }

    // セル更新（immerによる構造共有）
    auto new_row = this->cells[pos.row].set(pos.column, updated_cell_result.value());
    auto new_cells = this->cells.set(pos.row, new_row);

    return TetrisGrid{id, position, size, grid_size, cell_factory, new_cells};
}
