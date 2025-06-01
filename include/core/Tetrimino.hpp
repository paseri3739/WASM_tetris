// ============================================================================
// tetrimino17.hpp   (C++17)
// ============================================================================

#ifndef F6A587CF_4593_4E2A_954D_DB3B83779411
#define F6A587CF_4593_4E2A_954D_DB3B83779411

#include <array>
#include <cstddef>
#include <cstdint>

#include <core/Position.hpp>        // Position {int x, y;}
#include <core/graphics_types.hpp>  // Color {uint8_t r,g,b,a;}

/**
 * TetriminoType
 * テトリミノの種類を表す列挙型
 */
enum class TetriminoType : std::uint8_t { I, O, T, S, Z, J, L };

/**
 * Rotation
 * テトリミノの回転状態を表す列挙型
 * - R0: 0度回転
 * - R90: 90度回転
 * - R180: 180度回転
 * - R270: 270度回転
 */
enum class Rotation : std::uint8_t { R0, R90, R180, R270 };

/**
 * TetriminoStateType
 * テトリミノの状態を表す列挙型
 * - ACTIVE: 現在操作中のテトリミノ
 * - PENDING: 設置待ちのテトリミノ
 * - LOCKED: 落下が完了し、固定されたテトリミノ
 */
enum class TetriminoStateType : std::uint8_t { ACTIVE, PENDING, LOCKED };

// ────────────────── テーブル定義 ──────────────────
namespace tetrimino {

constexpr std::array<Color, 7> kColors{{/* I */ {0, 255, 255, 255}, /* O */ {255, 255, 0, 255},
                                        /* T */ {255, 0, 255, 255}, /* S */ {0, 255, 0, 255},
                                        /* Z */ {255, 0, 0, 255}, /* J */ {0, 0, 255, 255},
                                        /* L */ {255, 165, 0, 255}}};
constexpr Color color_of(TetriminoType t) noexcept { return kColors[static_cast<std::size_t>(t)]; }

using Shape4 = std::array<std::array<bool, 4>, 4>;

constexpr std::array<Shape4, 7> kBaseShapes{{/* I */ {{{false, false, false, false},
                                                       {true, true, true, true},
                                                       {false, false, false, false},
                                                       {false, false, false, false}}},
                                             /* O */
                                             {{{true, true, false, false},
                                               {true, true, false, false},
                                               {false, false, false, false},
                                               {false, false, false, false}}},
                                             /* T */
                                             {{{false, true, false, false},
                                               {true, true, true, false},
                                               {false, false, false, false},
                                               {false, false, false, false}}},
                                             /* S */
                                             {{{false, true, true, false},
                                               {true, true, false, false},
                                               {false, false, false, false},
                                               {false, false, false, false}}},
                                             /* Z */
                                             {{{true, true, false, false},
                                               {false, true, true, false},
                                               {false, false, false, false},
                                               {false, false, false, false}}},
                                             /* J */
                                             {{{true, false, false, false},
                                               {true, true, true, false},
                                               {false, false, false, false},
                                               {false, false, false, false}}},
                                             /* L */
                                             {{{false, false, true, false},
                                               {true, true, true, false},
                                               {false, false, false, false},
                                               {false, false, false, false}}}}};

constexpr Shape4 rotate_cw(const Shape4& s) noexcept {
    Shape4 out{};
    for (std::size_t y = 0; y < 4; ++y)
        for (std::size_t x = 0; x < 4; ++x) out[x][3 - y] = s[y][x];
    return out;
}

/// 回転を都度計算（LUT 不使用）
inline Shape4 shape_of(TetriminoType type, Rotation r) noexcept {
    Shape4 s = kBaseShapes[static_cast<std::size_t>(type)];
    switch (r) {
        case Rotation::R0:
            return s;
        case Rotation::R90:
            return rotate_cw(s);
        case Rotation::R180:
            return rotate_cw(rotate_cw(s));
        case Rotation::R270:
            return rotate_cw(rotate_cw(rotate_cw(s)));
    }
    return s;  // 対応外 → R0
}

constexpr std::uint32_t kDefaultLockDelayMs = 50;

}  // namespace tetrimino

// ────────────────── 実行時オブジェクト ──────────────────
struct Tetrimino {
    Position pos{};
    TetriminoType type{TetriminoType::I};
    Rotation rot{Rotation::R0};
    TetriminoStateType state{TetriminoStateType::ACTIVE};
    std::uint32_t lock_elapsed_ms{0};
};

// ────────────────── 操作ユーティリティ ──────────────────
namespace tetrimino {

[[nodiscard]] inline Tetrimino make(Position p, TetriminoType t,
                                    TetriminoStateType st = TetriminoStateType::ACTIVE) noexcept {
    return {p, t, Rotation::R0, st, 0};
}

[[nodiscard]] inline Tetrimino move(const Tetrimino& src, int dx, int dy) noexcept {
    auto out = src;
    out.pos.x += dx;
    out.pos.y += dy;
    return out;
}
[[nodiscard]] inline Tetrimino drop(const Tetrimino& src) noexcept { return move(src, 0, 1); }
[[nodiscard]] inline Tetrimino rotate_cw(const Tetrimino& src) noexcept {
    auto out = src;
    out.rot = static_cast<Rotation>((static_cast<std::uint8_t>(src.rot) + 1) & 3);
    return out;
}
[[nodiscard]] inline Tetrimino add_lock_elapsed(const Tetrimino& src, std::uint32_t dt) noexcept {
    if (src.state != TetriminoStateType::PENDING) return src;
    auto out = src;
    out.lock_elapsed_ms += dt;
    return out;
}
[[nodiscard]] inline Tetrimino reset_lock_elapsed(const Tetrimino& src) noexcept {
    if (src.state != TetriminoStateType::PENDING) return src;
    auto out = src;
    out.lock_elapsed_ms = 0;
    return out;
}

}  // namespace tetrimino

#endif /* F6A587CF_4593_4E2A_954D_DB3B83779411 */
