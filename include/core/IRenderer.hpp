#ifndef B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#define B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#include <core/Position.hpp>
#include <core/graphics_types.hpp>  // 先ほどのプリミティブ
#include <string>

class IRenderer {
   public:
    virtual ~IRenderer() = default;

    // フレーム制御
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;

    // 画面クリア
    virtual void clear(Color color = {0, 0, 0, 255}) = 0;

    // プリミティブ描画
    virtual void draw_rect(const Rect& rect, Color color, bool filled = true) = 0;
    virtual void draw_line(Position start, Position end, Color color) = 0;

    // 他にも draw_texture, draw_text など必要に応じて追加
    virtual void draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                              double angle = 0.0) = 0;
};

#endif /* B8AC84C6_6A5A_4990_8095_F03C1115A0EC */
