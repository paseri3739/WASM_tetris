#ifndef FDF1996A_28A1_4BB6_9350_B2E7149BB007
#define FDF1996A_28A1_4BB6_9350_B2E7149BB007

#include <core/Position.hpp>
#include <cstdint>
#include <string>

/** RGBA カラー（0-255 範囲） */
struct Color {
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};
    static Color from_string(std::string color_str);
};

/** 幅・高さを表す 2D サイズ
 *  - width: 幅
 *  - height: 高さ
 */
struct Size {
    double width;
    double height;
};

/** 位置とサイズを組み合わせた矩形
 *   - pos: 左上隅の位置
 *   - size: 幅と高さ
 */
struct Rect {
    Position pos;
    Size size;
};

/** バックエンド依存のテクスチャ識別子 */
using TextureId = std::uint32_t;

#endif /* FDF1996A_28A1_4BB6_9350_B2E7149BB007 */
