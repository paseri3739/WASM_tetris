#ifndef EAEC85BA_F694_47C7_AB0E_2FD093BD0A16
#define EAEC85BA_F694_47C7_AB0E_2FD093BD0A16

#include <algorithm>
#include <core/Cell.hpp>
#include <core/IRenderer.hpp>
#include <core/Position.hpp>
#include <core/Tetrimino.hpp>
#include <core/graphics_types.hpp>
#include <string>
#include <tl/expected.hpp>
#include <vector>

/**
 * TetrisGrid ― テトリスの盤面を表す値オブジェクト
 *   - 生成は static create() からのみ許可
 *   - 不変オブジェクトとみなし setter は用意しない
 */
class TetrisGrid {
   public:
    // 読み取り専用でpublicにしておく
    const std::string id;
    const Position position;                     ///< グリッドの左上位置
    const Size size;                             ///< 全体サイズ
    const GridColumnRow grid_size;               ///< 行数・列数
    const std::vector<std::vector<Cell>> cells;  ///< 各セルの状態
    const CellFactory cell_factory;              ///< セル生成用ファクトリ

    /**
     * コンストラクタ
     * @param id グリッドの識別子
     * @param position グリッドの左上位置
     * @param size グリッドのサイズ
     * @param grid_size 行数・列数
     * @param cell_factory セル生成用ファクトリ
     */
    TetrisGrid(std::string id, Position position, Size size, GridColumnRow grid_size,
               CellFactory cell_factory)
        : id(std::move(id)),
          position(position),
          size(size),
          grid_size(grid_size),
          cell_factory(std::move(cell_factory)),
          cells(initialize_cells(this->position, this->grid_size, this->cell_factory)) {}

    inline void render(IRenderer& renderer) {
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

    Position get_position_of_cell(const GridColumnRow& grid_position, double cell_size);

    GridColumnRow get_grid_position_of_cell(const Position& cell_position, double cell_size);

    bool is_within_bounds(int column, int row);

    bool is_within_bounds(const Position& position);

    bool is_filled_cell(const GridColumnRow& grid_position) const;

   private:
    static std::vector<std::vector<Cell>> initialize_cells(const Position& origin,
                                                           const GridColumnRow& grid_size,
                                                           const CellFactory& factory) {
        std::vector<std::vector<Cell>> result(grid_size.row);
        for (int row = 0; row < grid_size.row; ++row) {
            std::vector<Cell> row_cells;
            for (int col = 0; col < grid_size.column; ++col) {
                Position cell_pos{
                    origin.x + col * factory.size.width,
                    origin.y + row * factory.size.height,
                };
                row_cells.push_back(
                    factory.create(cell_pos, CellStatus::EMPTY, Color::from_string("white")));
            }
            result[row] = std::move(row_cells);
        }
        return result;
    }
};

#endif
