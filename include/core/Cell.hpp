#ifndef B46CA402_5D14_4D1D_9923_49018BA7FA61
#define B46CA402_5D14_4D1D_9923_49018BA7FA61

#include <core/GameConfig.hpp>
#include <core/Position.hpp>
#include <string>
#include <tl/expected.hpp>

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
    const double size;
    const std::string color;

    // コンストラクタは private にして、CellFactory 経由でのみインスタンス化可能にする
   private:
    Cell(CellStatus t, Position pos, double sz, std::string col)
        : type(t), position(pos), size(sz), color(std::move(col)) {}
    friend class CellFactory;  // CellFactoryからのみインスタンス化可能
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

// constexprとstatic_assertを組み合わせることで状態遷移の妥当性チェックをコンパイル時に行う。単体テストを不要にできる。
static_assert(is_legal_transition(CellStatus::EMPTY, CellStatus::MOVING));
static_assert(is_legal_transition(CellStatus::MOVING, CellStatus::EMPTY));
static_assert(is_legal_transition(CellStatus::MOVING, CellStatus::FILLED));
static_assert(is_legal_transition(CellStatus::FILLED, CellStatus::EMPTY));

/**
 * CellFactory ― 値オブジェクト生成器
 */
class CellFactory {
   public:
    explicit CellFactory(const GameConfig& cfg) : size_{cfg.cell.size} {}

    [[nodiscard]] Cell create(const Position& pos, CellStatus type, std::string color) const {
        if (type == CellStatus::EMPTY) color = "white";
        return Cell{type, pos, size_, std::move(color)};
    }

    int cell_size() const noexcept { return size_; }

    /**
     * セルの状態を更新する
     * @param cell 更新対象のセル
     * @param new_state 新しい状態
     * @param new_color 新しい色（EMPTYの場合は常に白）
     * @return 成功時は更新後のセル、失敗時はエラーメッセージ
     */
    tl::expected<Cell, std::string> update_cell_state(const Cell& cell, CellStatus new_state,
                                                      std::string new_color);

   private:
    double size_;
};

#endif /* B46CA402_5D14_4D1D_9923_49018BA7FA61 */
