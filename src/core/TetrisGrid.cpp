#include <core/TetrisGrid.hpp>

void TetrisGrid::render(IRenderer& renderer) {
    // セルを描画する
    int columns = this->grid_size.column;
    int rows = this->grid_size.row;

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const Cell& cell = this->cells[row][column];
            cell.render(renderer);
        }
    }
}

Position tetris_grid_manipulation::get_position_of_cell(const TetrisGrid& grid,
                                                        const GridColumnRow& grid_position,
                                                        double cell_size) {
    // グリッドの左上位置に、セルの位置を加算してセルの位置を計算
    return Position{grid.position.x + grid_position.column * cell_size,
                    grid.position.y + grid_position.row * cell_size};
}

GridColumnRow tetris_grid_manipulation::get_grid_position_of_cell(const TetrisGrid& grid,
                                                                  const Position& cell_position,
                                                                  double cell_size) {
    // グリッドの左上位置からセルの位置を引いて、セルの列と行を計算
    int column = static_cast<int>((cell_position.x - grid.position.x) / cell_size);
    int row = static_cast<int>((cell_position.y - grid.position.y) / cell_size);
    return GridColumnRow{column, row};
}

TetrisGrid tetris_grid_manipulation::map_tetrimino_to_grid(const TetrisGrid& grid,
                                                           const Tetrimino& tetrimino,
                                                           CellFactory& cell_factory) {
    std::terminate();  // TODO: 呼ばれると即終了
}

TetrisGrid tetris_grid_manipulation::unmap_moving_cell(const TetrisGrid& grid,
                                                       const CellFactory& cell_factory) {
    std::terminate();  // TODO: 呼ばれると即終了
}

TetrisGrid tetris_grid_manipulation::clear_filled_rows(const TetrisGrid& grid,
                                                       CellFactory& cell_factory) {
    std::terminate();  // TODO: 呼ばれると即終了
}
