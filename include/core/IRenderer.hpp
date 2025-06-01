#ifndef B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#define B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#include <core/Position.hpp>
#include <core/graphics_types.hpp>  // 先ほどのプリミティブ
#include <string>
#include <tl/expected.hpp>

using TextureId = std::uint32_t;
using FontId = std::uint32_t;

/**
 * IRenderer ― レンダリングのインターフェース
 * 描画処理を抽象化するインターフェースで、具体的なレンダリングバックエンドに依存しない。
 * このインターフェースを実装することで、異なるレンダリングバックエンド（SDL, OpenGL,
 * Vulkanなど）に対応可能。 各種プリミティブ描画やテクスチャ描画のメソッドを提供する。
 */
class IRenderer {
   public:
    virtual ~IRenderer() = default;

    // フレーム制御
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;

    // 画面クリア
    virtual void clear(Color color) = 0;

    // プリミティブ描画
    virtual void fill_rect(const Rect&, Color) = 0;
    virtual void stroke_rect(const Rect&, Color) = 0;
    virtual void draw_line(Position start, Position end, Color color) = 0;

    // 他にも draw_texture, draw_text など必要に応じて追加
    virtual void draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                              double angle) = 0;

    // ──────────── フォント関連 ------------------------------------------------
    /**
     * フォントをロードして登録
     * @param path     フォントファイル（TTF/OTF など）
     * @param pt_size  ポイントサイズ
     * @return 成功: FontId, 失敗: エラーメッセージ
     */
    [[nodiscard]]
    virtual tl::expected<FontId, std::string> register_font(const std::string& path,
                                                            int pt_size) = 0;

    /**
     * テキストを即描画
     *   - キャッシュが必要なら呼び出し側で wrap してください
     * @param font_id 登録済みフォント
     * @param utf8    UTF-8 文字列
     * @param pos     描画左上座標
     * @param color   RGBA
     * @return 成功: void, 失敗: エラーメッセージ
     */
    [[nodiscard]]
    virtual tl::expected<void, std::string> draw_text(FontId font_id, const std::string& utf8,
                                                      Position pos, Color color) = 0;
};

#endif /* B8AC84C6_6A5A_4990_8095_F03C1115A0EC */
