#ifndef F6A587CF_4593_4E2A_954D_DB3B83779411
#define F6A587CF_4593_4E2A_954D_DB3B83779411

#include <array>
#include <core/graphics_types.hpp>  // struct Color{r,g,b,a}
#include <cstddef>                  // std::size_t
#include <cstdint>                  // std::uint8_t

/// テトリス 7 種のテトロミノ
enum class TetriminoType : std::uint8_t { I, O, T, S, Z, J, L };

namespace tetrimino {

/// 形状 → 色 のテーブル（添字と enum 値を一致させる）
constexpr std::array<Color, 7> kColors{{
    /* I */ {0, 255, 255, 255},  // cyan
    /* O */ {255, 255, 0, 255},  // yellow
    /* T */ {255, 0, 255, 255},  // magenta
    /* S */ {0, 255, 0, 255},    // green
    /* Z */ {255, 0, 0, 255},    // red
    /* J */ {0, 0, 255, 255},    // blue
    /* L */ {255, 165, 0, 255}   // orange
}};

/// テトロミノ種別から色を取得
constexpr Color color_of(TetriminoType type) noexcept {
    return kColors[static_cast<std::size_t>(type)];
}

// ――――― コンパイル時整合性チェック ―――――
static_assert(kColors.size() == 7, "Tetromino color table must have 7 entries.");
static_assert(static_cast<std::size_t>(TetriminoType::L) == 6,
              "Enum order and color table order must match.");

}  // namespace tetrimino

#endif /* F6A587CF_4593_4E2A_954D_DB3B83779411 */
