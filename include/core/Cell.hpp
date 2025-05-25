#ifndef B46CA402_5D14_4D1D_9923_49018BA7FA61
#define B46CA402_5D14_4D1D_9923_49018BA7FA61

#include <Position.hpp>
#include <string>
#include <tl/expected.hpp>
#include "GameConfig.hpp"

/**
 * セル状態
 */
enum class CellStatus { EMPTY, MOVING, FILLED };

/**
 * セル値オブジェクト
 *  - メンバは public immutable（直接代入せず生成関数経由で作成）
 */
struct Cell {
    const CellStatus type;
    const Position position;
    const double cellWidth;
    const double cellHeight;
    const std::string color;
};

/**
 * 状態遷移が妥当か判定
 */
constexpr bool is_legal_transition(CellStatus from, CellStatus to) noexcept {
    switch (from) {
        case CellStatus::EMPTY:
            return to == CellStatus::MOVING;
        case CellStatus::MOVING:
            return to == CellStatus::EMPTY || to == CellStatus::FILLED;
        case CellStatus::FILLED:
            return to == CellStatus::EMPTY;
        default:
            return false;
    }
}

// constexprとstatic_assertを組み合わせることで状態遷移の妥当性チェックをコンパイル時に行う
static_assert(is_legal_transition(CellStatus::EMPTY, CellStatus::MOVING));
static_assert(is_legal_transition(CellStatus::MOVING, CellStatus::EMPTY));
static_assert(is_legal_transition(CellStatus::MOVING, CellStatus::FILLED));
static_assert(is_legal_transition(CellStatus::FILLED, CellStatus::EMPTY));

/**
 * セル状態更新（純粋関数）
 *  - 成功時: 新しい Cell
 *  - 失敗時: エラーメッセージ
 */
inline tl::expected<Cell, std::string> update_cell_state(const Cell& cell, CellStatus new_state,
                                                         std::string new_color) {
    if (!is_legal_transition(cell.type, new_state)) {
        return tl::unexpected{"illegal state transition"};
    }

    // Empty → 常に白
    if (new_state == CellStatus::EMPTY) new_color = "white";

    return Cell{new_state, cell.position, cell.cellWidth, cell.cellHeight, std::move(new_color)};
}

/**
 * CellFactory ― 値オブジェクト生成器
 */
class CellFactory {
   public:
    explicit CellFactory(const GameConfig& cfg) : w_{cfg.cell.size}, h_{cfg.cell.size} {}

    [[nodiscard]] Cell create(const Position& pos, CellStatus type, std::string color) const {
        if (type == CellStatus::EMPTY) color = "white";
        return Cell{type, pos, w_, h_, std::move(color)};
    }

    int cell_width() const noexcept { return w_; }
    int cell_height() const noexcept { return h_; }

   private:
    double w_;
    double h_;
};

#endif /* B46CA402_5D14_4D1D_9923_49018BA7FA61 */
