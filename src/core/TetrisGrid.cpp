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

tl::expected<TetrisGrid, std::string> tetris_grid_manipulation::make_all_empty(
    const TetrisGrid& grid, CellFactory& cell_factory) {
    std::vector<std::vector<Cell>> new_cells;

    for (const auto& row : grid.cells) {
        std::vector<Cell> new_row;
        for (const auto& cell : row) {
            auto new_cell =
                cell_factory.create(cell.position, CellStatus::EMPTY, Color::from_string("white"));
            new_row.push_back(new_cell);
        }
        new_cells.push_back(std::move(new_row));
    }

    // 新しいTetrisGridをcreateで構築（privateコンストラクタのため直接生成できない）
    return TetrisGrid::create(grid.id, grid.position, grid.size,
                              static_cast<int>(grid.grid_size.row),
                              static_cast<int>(grid.grid_size.column))
        .and_then([&](TetrisGrid new_grid) -> tl::expected<TetrisGrid, std::string> {
            // セルだけ差し替える
            new_grid.cells = std::move(new_cells);
            return new_grid;
        });
}
