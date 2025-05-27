#ifndef B9A2347A_606F_4127_89AC_A0403C3E1235
#define B9A2347A_606F_4127_89AC_A0403C3E1235

#include <SDL2/SDL.h>
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
    void clear(Color color = {0, 0, 0, 255}) override;

    // プリミティブ描画 ------------------------------------------------------
    void draw_rect(const Rect& rect, Color color, bool filled = true) override;

    void draw_line(Position start, Position end, Color color) override;

    void draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                      double angle = 0.0) override;

    // 追加ユーティリティ ----------------------------------------------------
    /** SDL_Texture* を登録し TextureId を取得 */
    [[nodiscard]]
    tl::expected<TextureId, std::string> register_texture(SDL_Texture* tex);

    /** BlendMode を直接設定 */
    void set_blend_mode(SDL_BlendMode mode) { SDL_SetRenderDrawBlendMode(renderer_, mode); }

    /** 外部連携用に生ポインタを公開 */
    SDL_Renderer* raw() noexcept { return renderer_; }

   private:
    // create() からのみ呼ばれるプライベートコンストラクタ
    SDLRenderer(SDL_Window* window, SDL_Renderer* renderer) noexcept
        : window_{window}, renderer_{renderer} {}

    SDL_Window* window_{nullptr};      // 借用：破棄はアプリ側
    SDL_Renderer* renderer_{nullptr};  // 保有：デストラクタで破棄

    std::unordered_map<TextureId, SDL_Texture*> textures_;
    TextureId next_id_{1};

    // 内部ヘルパ ------------------------------------------------------------
    void set_draw_color(Color c) { SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a); }
};

#endif /* D17E6D9B_A7F3_4DDC_97F4_93C7522C8A8C */
