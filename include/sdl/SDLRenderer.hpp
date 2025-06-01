#ifndef B9A2347A_606F_4127_89AC_A0403C3E1235
#define B9A2347A_606F_4127_89AC_A0403C3E1235

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <core/IRenderer.hpp>
#include <memory>
#include <string>
#include <tl/expected.hpp>
#include <unordered_map>

/**
 * SDLRenderer ― SDL2 バックエンド実装
 *   - RAII で SDL_Renderer を保持
 *   - 生成は static create() から行い、結果を tl::expected で返す
 *   - テクスチャは register_texture() で管理（これも expected で結果返却）
 */
class SDLRenderer final : public IRenderer {
   public:
    /**
     * ファクトリ関数
     * @param window SDL_Window*（借用）
     * @param flags  SDL_Renderer フラグ
     * @return 成功時: std::unique_ptr<SDLRenderer>
     *         失敗時: エラーメッセージ
     */
    static tl::expected<std::unique_ptr<SDLRenderer>, std::string> create(
        SDL_Window* window, int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // コピー禁止・ムーブのみ許可
    SDLRenderer(const SDLRenderer&) = delete;
    SDLRenderer& operator=(const SDLRenderer&) = delete;
    SDLRenderer(SDLRenderer&&) = default;
    SDLRenderer& operator=(SDLRenderer&&) = default;

    ~SDLRenderer() override;

    // フレーム制御 ----------------------------------------------------------
    void begin_frame() override;
    void end_frame() override;

    // 画面クリア ------------------------------------------------------------
    void clear(Color color) override;

    // プリミティブ描画 ------------------------------------------------------
    void fill_rect(const Rect& rect, Color color) override;    // 塗りつぶし
    void stroke_rect(const Rect& rect, Color color) override;  // 枠線のみ

    void draw_line(Position start, Position end, Color color) override;

    void draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                      double angle = 0.0) override;

    // ──────────── フォント関連 ------------------------------------------------
    /**
     * フォントをロードして登録
     * @param path     フォントファイル（TTF/OTF など）
     * @param pt_size  ポイントサイズ
     * @return 成功: FontId, 失敗: エラーメッセージ
     */
    [[nodiscard]]
    tl::expected<FontId, std::string> register_font(const std::string& path, int pt_size) override;

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
    tl::expected<void, std::string> draw_text(FontId font_id, const std::string& utf8, Position pos,
                                              Color color) override;

    // 追加ユーティリティ ----------------------------------------------------
    /** SDL_Texture* を登録し TextureId を取得 */
    [[nodiscard]]
    tl::expected<TextureId, std::string> register_texture(SDL_Texture* tex);

    /** BlendMode を直接設定 */
    void set_blend_mode(SDL_BlendMode mode) const { SDL_SetRenderDrawBlendMode(renderer_, mode); }

    /** 外部連携用に生ポインタを公開 */
    [[nodiscard]]
    SDL_Renderer* raw() const noexcept {
        return renderer_;
    }

   private:
    // create() からのみ呼ばれるプライベートコンストラクタ
    SDLRenderer(SDL_Window* window, SDL_Renderer* renderer) noexcept
        : window_{window}, renderer_{renderer} {}

    SDL_Window* window_{nullptr};      // 借用：破棄はアプリ側
    SDL_Renderer* renderer_{nullptr};  // 保有：デストラクタで破棄

    std::unordered_map<TextureId, SDL_Texture*> textures_;
    TextureId next_id_{1};

    // テクスチャと同じ管理方式
    std::unordered_map<FontId, TTF_Font*> fonts_;
    FontId next_font_id_{0};

    // 内部ヘルパ ------------------------------------------------------------
    void set_draw_color(Color c) { SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a); }
};

#endif /* D17E6D9B_A7F3_4DDC_97F4_93C7522C8A8C */
