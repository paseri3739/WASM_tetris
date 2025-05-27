#ifndef B9A2347A_606F_4127_89AC_A0403C3E1235
#define B9A2347A_606F_4127_89AC_A0403C3E1235

#include <SDL2/SDL.h>
#include <core/IRenderer.hpp>
#include <stdexcept>
#include <unordered_map>

/**
 * SDLRenderer ― SDL2 バックエンド実装
 *   - RAII で SDL_Renderer を保持
 *   - テクスチャは register_texture() で管理
 *   - begin_frame()/end_frame() により Present を明示
 */
class SDLRenderer final : public IRenderer {
   public:
    explicit SDLRenderer(SDL_Window* window,
                         int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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
    /** SDL_Texture* を登録し TextureId を取得（SDLRenderer が破棄を担当） */
    [[nodiscard]]
    TextureId register_texture(SDL_Texture* tex);

    /** BlendMode を直接設定 */
    void set_blend_mode(SDL_BlendMode mode) { SDL_SetRenderDrawBlendMode(renderer_, mode); }

    /** 外部連携用に生ポインタを公開 */
    SDL_Renderer* raw() noexcept { return renderer_; }

   private:
    SDL_Window* window_{nullptr};      // 借用：破棄はアプリ側
    SDL_Renderer* renderer_{nullptr};  // 保有：デストラクタで破棄

    std::unordered_map<TextureId, SDL_Texture*> textures_;
    TextureId next_id_{1};

    // 内部ヘルパ ------------------------------------------------------------
    void set_draw_color(Color c) { SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a); }
};

#endif /* D17E6D9B_A7F3_4DDC_97F4_93C7522C8A8C */
