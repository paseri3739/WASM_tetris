#ifndef B6A2B782_FE61_4667_84AD_802250E4C851
#define B6A2B782_FE61_4667_84AD_802250E4C851

#include <Cell.hpp>
#include <GameConfig.hpp>
#include <Position.hpp>

struct TetrisGrid {
    std::string name;
    Position position;
    double width;
    double height;
    int rows;
    int columns;
    std::vector<std::vector<Cell>> cells;

   private:
    // コンストラクタは外部から呼ばれないようにprivateにする
    TetrisGrid(const std::string& n, const Position& pos, double w, double h, int r, int c,
               std::vector<std::vector<Cell>> cs)
        : name(n), position(pos), width(w), height(h), rows(r), columns(c), cells(std::move(cs)) {}
    friend class TetrisGridFactory;  // TetrisGridFactoryからのみインスタンス化可能
};

class TetrisGridFactory {
   private:
    CellFactory cell_factory;
    GameConfig game_config;

   public:
    TetrisGridFactory(CellFactory cf, GameConfig gc)
        : cell_factory(std::move(cf)), game_config(std::move(gc)) {}

    TetrisGrid create_grid(const std::string& name, const Position& position, double width,
                           double height, int rows, int columns) {
        TetrisGrid grid{name, position, width, height, rows, columns, {}};
        grid.cells.resize(rows, std::vector<Cell>(columns));
        return grid;
    }
};

#endif /* B6A2B782_FE61_4667_84AD_802250E4C851 */
