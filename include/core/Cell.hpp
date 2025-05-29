#ifndef B46CA402_5D14_4D1D_9923_49018BA7FA61
#define B46CA402_5D14_4D1D_9923_49018BA7FA61

#include <core/GameConfig.hpp>
#include <core/IRenderer.hpp>
#include <core/Position.hpp>
#include <core/graphics_types.hpp>
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
    const Size size;
    const Color color;

    inline void render(IRenderer& renderer) const {
        // 描画処理の実装
        Rect rect{this->position.x, this->position.y, this->size.width, this->size.height};
        if (this->type == CellStatus::FILLED) {
            renderer.fill_rect(rect, this->color);
        } else {
            renderer.stroke_rect(rect, Color::from_string("black"));
        }
    };

    // コンストラクタは private にして、CellFactory 経由でのみインスタンス化可能にする
   private:
    Cell(CellStatus t, Position pos, Size sz, Color col)
        : type(t), position(pos), size(sz), color(col) {}
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
    const Size size;
    explicit CellFactory(const GameConfig& cfg) : size{cfg.cell.size} {}

    [[nodiscard]] Cell create(const Position& pos, CellStatus type, Color color) const;

    /**
     * セルの状態を更新する
     * @param cell 更新対象のセル
     * @param new_state 新しい状態
     * @param new_color 新しい色（EMPTYの場合は常に白）
     * @return 成功時は更新後のセル、失敗時はエラーメッセージ
     */
    tl::expected<Cell, std::string> update_cell_state(const Cell& cell, CellStatus new_state,
                                                      Color new_color);
};

#endif /* B46CA402_5D14_4D1D_9923_49018BA7FA61 */
