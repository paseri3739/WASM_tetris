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
    virtual void draw_point(const Position& pos, Color color) = 0;

    /**
     * 複数の点をバッチで描画する
     * @param points   描画する座標の配列
     * @param count    要素数
     * @param color    共通の色 (RGBA)
     */
    virtual void draw_points(const Position* points, int count, Color color) = 0;

    /**
     * 複数の線分をバッチで描画する
     * @param segments 描画する線分の配列 (start,end) が交互に並ぶなど自由に定義
     * @param count    線分数
     * @param color    共通の色 (RGBA)
     */
    virtual void draw_lines(const Position* segments, int count, Color color) = 0;

    /**
     * 複数の塗りつぶし長方形をバッチで描画する
     * @param rects    Rect 配列
     * @param count    要素数
     * @param color    共通の色 (RGBA)
     */
    virtual void fill_rects(const Rect* rects, int count, Color color) = 0;

    /**
     * テクスチャを即時描画
     * @param id     * @param src_region     * @param dst_region
     * @param angle
     */
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

    /**
     * 論理解像度を設定する
     * 例：320×240 でゲームロジックをまわし、ウィンドウには拡大して表示する場合に使う
     */
    virtual void set_logical_size(int width, int height) = 0;

    /**
     * ビューポート（描画できる領域）を指定する（クリップ矩形の設定など）
     */
    virtual void set_viewport(const Rect& clip_rect) = 0;

    /**
     * X/Y スケールを指定する
     * src座標→dst座標の倍率をプログラム側で制御したい場合に用いる
     */
    virtual void set_scale(double scale_x, double scale_y) = 0;

    /**
     * ファイルパスからテクスチャを生成・登録する
     * （内部で SDL_image などを使って surface を生成し、Texture を作成）
     */
    [[nodiscard]]
    virtual tl::expected<TextureId, std::string> register_texture_from_file(
        const std::string& path) = 0;

    /**
     * 生データ (RGBA8 など) からテクスチャを生成・登録する
     */
    [[nodiscard]]
    virtual tl::expected<TextureId, std::string> register_texture_from_memory(
        int width, int height, const unsigned char* pixel_data) = 0;

    /**
     * 描画ターゲットを切り替える
     * @param tex_id   nullptr の場合はデフォルト（ウィンドウ）に戻す
     */
    virtual tl::expected<void, std::string> set_render_target(TextureId tex_id) = 0;

    /**
     * フォントで指定した文字列 (UTF-8) の描画サイズを求める
     * @return (幅, 高さ) をペアで返す
     */
    virtual std::pair<int, int> measure_text(FontId font_id, const std::string& utf8) = 0;

    /**
     * テキストキャッシュ
     * @param font_id     * @param utf8     * @param color
     * @return
     */
    virtual tl::expected<TextureId, std::string> create_text_texture(FontId font_id,
                                                                     const std::string& utf8,
                                                                     Color color) = 0;
};

#endif /* B8AC84C6_6A5A_4990_8095_F03C1115A0EC */
