#ifndef FDF1996A_28A1_4BB6_9350_B2E7149BB007
#define FDF1996A_28A1_4BB6_9350_B2E7149BB007

#include <core/Position.hpp>
#include <cstdint>

/** RGBA カラー（0-255 範囲） */
struct Color {
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};
};

/** 幅・高さを表す 2D サイズ */
struct Size {
    double w;
    double h;
};

/** 位置とサイズを組み合わせた矩形 */
struct Rect {
    Position pos;
    Size size;
};

/** バックエンド依存のテクスチャ識別子 */
using TextureId = std::uint32_t;

#endif /* FDF1996A_28A1_4BB6_9350_B2E7149BB007 */
