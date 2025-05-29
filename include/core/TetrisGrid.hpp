#ifndef EAEC85BA_F694_47C7_AB0E_2FD093BD0A16
#define EAEC85BA_F694_47C7_AB0E_2FD093BD0A16
#ifndef F7B71047_1F3E_482C_9849_3CB6B22E22C3
#define F7B71047_1F3E_482C_9849_3CB6B22E22C3

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
    std::string id;
    Position position;                     ///< グリッドの左上位置
    Size size;                             ///< 全体サイズ
    GridColumnRow grid_size;               ///< 行数・列数
    std::vector<std::vector<Cell>> cells;  ///< 各セルの状態

    /**
     * ファクトリ関数
     * @param id   グリッドの識別子
     * @param pos  左上座標
     * @param sz   グリッド全体の描画サイズ
     * @param rows 行数
     * @param cols 列数
     * @return 成功: TetrisGrid / 失敗: エラーメッセージ
     */
    static tl::expected<TetrisGrid, std::string> create(const std::string& id, Position pos,
                                                        Size sz, int rows, int cols);

    inline void TetrisGrid::render(IRenderer& renderer) {
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

   private:
    // プライベートコンストラクタで外部からの直接生成を禁止
    TetrisGrid(std::string id_, Position pos_, Size sz_, GridColumnRow grid_sz_,
               std::vector<std::vector<Cell>> cells_)
        : id{std::move(id_)},
          position{pos_},
          size{sz_},
          grid_size{grid_sz_},
          cells{std::move(cells_)} {}

    // 旧 tetris_grid::create() は削除するため宣言しない
};

namespace tetris_grid_manipulation {

Position get_position_of_cell(const TetrisGrid& grid, const GridColumnRow& grid_position,
                              double cell_size);

GridColumnRow get_grid_position_of_cell(const TetrisGrid& grid, const Position& cell_position,
                                        double cell_size);

TetrisGrid map_tetrimino_to_grid(const TetrisGrid& grid, const Tetrimino& tetrimino,
                                 CellFactory& cell_factory);

TetrisGrid unmap_moving_cell(const TetrisGrid& grid, const CellFactory& cell_factory);

TetrisGrid clear_filled_rows(const TetrisGrid& grid, CellFactory& cell_factory);

bool is_within_bounds(const TetrisGrid& grid, int column, int row);

bool is_within_bounds(const TetrisGrid& grid, const Tetrimino& tetrimino);

typedef struct {
    bool collided;
    bool horizontal;
} CollisionResult;

CollisionResult check_collision(const TetrisGrid& grid, const Tetrimino& next,
                                const Tetrimino& previous);

bool is_collided_bottom(const TetrisGrid& grid, const Tetrimino& tetrimino);

bool is_game_over(const TetrisGrid& grid, const Tetrimino& tetrimino);

TetrisGrid fix_tetrimino(const TetrisGrid& grid, const Tetrimino& tetrimino);

std::vector<GridColumnRow> get_tetrimino_map(const TetrisGrid& grid, const Tetrimino& tetrimino);

std::vector<GridColumnRow> get_tetrimino_map_with_out_of_bounds(const TetrisGrid& grid,
                                                                const Tetrimino& tetrimino);

}  // namespace tetris_grid_manipulation
#endif /* F7B71047_1F3E_482C_9849_3CB6B22E22C3 */

#endif /* EAEC85BA_F694_47C7_AB0E_2FD093BD0A16 */
