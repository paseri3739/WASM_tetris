#include "SDLRenderer.hpp"

// ──────────── ローカル変換ユーティリティ ────────────
namespace {
inline SDL_Rect to_sdl_rect(const Rect& r) {
    return SDL_Rect{static_cast<int>(r.pos.x), static_cast<int>(r.pos.y),
                    static_cast<int>(r.size.w), static_cast<int>(r.size.h)};
}
}  // namespace

// ──────────── コンストラクタ / デストラクタ ────────────
SDLRenderer::SDLRenderer(SDL_Window* window, int flags) : window_{window} {
    if (!window_) throw std::invalid_argument("SDLRenderer: window must not be null");

    renderer_ = SDL_CreateRenderer(window_, -1, flags);
    if (!renderer_) {
        throw std::runtime_error(std::string{"SDL_CreateRenderer failed: "} + SDL_GetError());
    }
}

SDLRenderer::~SDLRenderer() {
    for (auto& [id, tex] : textures_) {
        SDL_DestroyTexture(tex);
    }
    textures_.clear();

    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
}

// ──────────── フレーム制御 ────────────
void SDLRenderer::begin_frame() {
    /* 今のところ特別な処理なし。
       状態リセットや ImGui 等の前処理を挟む場合に利用 */
}

void SDLRenderer::end_frame() { SDL_RenderPresent(renderer_); }

// ──────────── 画面クリア ────────────
void SDLRenderer::clear(Color color) {
    set_draw_color(color);
    SDL_RenderClear(renderer_);
}

// ──────────── プリミティブ描画 ────────────
void SDLRenderer::draw_rect(const Rect& rect, Color color, bool filled) {
    set_draw_color(color);
    SDL_Rect srect = to_sdl_rect(rect);
    if (filled) {
        SDL_RenderFillRect(renderer_, &srect);
    } else {
        SDL_RenderDrawRect(renderer_, &srect);
    }
}

void SDLRenderer::draw_line(Position start, Position end, Color color) {
    set_draw_color(color);
    SDL_RenderDrawLine(renderer_, static_cast<int>(start.x), static_cast<int>(start.y),
                       static_cast<int>(end.x), static_cast<int>(end.y));
}

void SDLRenderer::draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                               double angle) {
    auto it = textures_.find(id);
    if (it == textures_.end()) return;  // 未登録 ID は無視（必要なら throw）

    SDL_Texture* tex = it->second;
    SDL_Rect src = to_sdl_rect(src_region);
    SDL_Rect dst = to_sdl_rect(dst_region);

    SDL_RenderCopyEx(renderer_, tex, &src, &dst, angle, nullptr, SDL_FLIP_NONE);
}

// ──────────── テクスチャ管理 ────────────
TextureId SDLRenderer::register_texture(SDL_Texture* tex) {
    if (!tex) throw std::invalid_argument("register_texture: texture is null");
    const TextureId id = next_id_++;
    textures_.emplace(id, tex);
    return id;
}
