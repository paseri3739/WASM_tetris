#ifndef EAEC85BA_F694_47C7_AB0E_2FD093BD0A16
#define EAEC85BA_F694_47C7_AB0E_2FD093BD0A16

#include <algorithm>
#include <core/Cell.hpp>
#include <core/IRenderer.hpp>
#include <core/Position.hpp>
#include <core/graphics_types.hpp>
#include <immer/vector.hpp>
#include <string>

/**
 * TetrisGrid ― テトリスの盤面を表す値オブジェクト
 *   - 生成は static create() からのみ許可
 *   - 不変オブジェクトとみなし setter は用意しない
 */
class TetrisGrid {
   public:
    // 読み取り専用でpublicにしておく
    const std::string id;
    const Position position;                         ///< グリッドの左上位置
    const Size size;                                 ///< 全体サイズ
    const GridColumnRow grid_size;                   ///< 行数・列数
    const immer::vector<immer::vector<Cell>> cells;  /// < 重い部分はimmer::vectorで管理
    const CellFactory cell_factory;                  ///< セル生成用ファクトリ

    TetrisGrid(std::string id, const Position& position, const Size& size, GridColumnRow grid_size,
               CellFactory factory, immer::vector<immer::vector<Cell>> cells)
        : id(std::move(id)),
          position(position),
          size(size),
          grid_size(grid_size),
          cells(std::move(cells)),
          cell_factory(std::move(factory)) {}

    inline void render(IRenderer& renderer) const {
        // セルを描画する
        const int columns = this->grid_size.column;
        const int rows = this->grid_size.row;

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                const Cell& cell = this->cells[row][column];
                cell.render(renderer);
            }
        }
    }

    Position get_position_of_cell(const GridColumnRow& grid_position, double cell_size) const;

    GridColumnRow get_grid_position_of_cell(const Position& cell_position, double cell_size) const;

    bool is_within_bounds(int column, int row) const;

    bool is_within_bounds(const Position& position) const;

    [[nodiscard]]
    bool is_filled_cell(const GridColumnRow& grid_position) const;

    [[nodiscard]]
    bool is_colliding(const GridColumnRow& before, const GridColumnRow& after) const;

    // 変更点：更新系メソッドは新しいインスタンスを返す
    [[nodiscard]] TetrisGrid update_cell(const GridColumnRow& pos, CellStatus status,
                                         Color color) const;

   private:
    inline static immer::vector<immer::vector<Cell>> initialize_cells(
        const Position& origin, const GridColumnRow& grid_size, const CellFactory& factory) {
        immer::vector<immer::vector<Cell>> rows;
        for (int row = 0; row < grid_size.row; ++row) {
            immer::vector<Cell> columns;
            for (int col = 0; col < grid_size.column; ++col) {
                Position cell_pos{
                    origin.x + col * factory.size.width,
                    origin.y + row * factory.size.height,
                };
                columns = columns.push_back(
                    factory.create(cell_pos, CellStatus::EMPTY, Color::from_string("white")));
            }
            rows = rows.push_back(columns);
        }
        return rows;
    }
};

#endif
