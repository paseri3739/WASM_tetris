#include <SDL2/SDL_ttf.h>
#include <sdl/SDLRenderer.hpp>

// ──────────── ローカル変換ユーティリティ ────────────
namespace {
inline SDL_Rect to_sdl_rect(const Rect& r) {
    return SDL_Rect{static_cast<int>(r.pos.x), static_cast<int>(r.pos.y),
                    static_cast<int>(r.size.width), static_cast<int>(r.size.height)};
}
}  // namespace

// ──────────── create (ファクトリ) ────────────
tl::expected<std::unique_ptr<SDLRenderer>, std::string> SDLRenderer::create(SDL_Window* window,
                                                                            int flags) {
    if (!window) {
        return tl::unexpected<std::string>{"SDLRenderer: window must not be null"};
    }

    // SDL_ttf 初期化
    if (TTF_WasInit() == 0 && TTF_Init() == -1) {
        return tl::unexpected<std::string>(std::string{"TTF_Init failed: "} + TTF_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, flags);
    if (!renderer) {
        return tl::unexpected<std::string>(std::string{"SDL_CreateRenderer failed: "} +
                                           SDL_GetError());
    }

    return std::unique_ptr<SDLRenderer>{new SDLRenderer{window, renderer}};
}

// ──────────── デストラクタ ────────────
SDLRenderer::~SDLRenderer() {
    // テクスチャ解放
    for (auto& [id, tex] : textures_) SDL_DestroyTexture(tex);
    textures_.clear();

    // フォント解放
    for (auto& [id, font] : fonts_) TTF_CloseFont(font);
    fonts_.clear();

    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (TTF_WasInit()) TTF_Quit();
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
void SDLRenderer::fill_rect(const Rect& rect, Color color) {
    set_draw_color(color);
    const SDL_Rect srect = to_sdl_rect(rect);
    SDL_RenderFillRect(renderer_, &srect);
}

void SDLRenderer::stroke_rect(const Rect& rect, Color color) {
    set_draw_color(color);
    const SDL_Rect srect = to_sdl_rect(rect);
    SDL_RenderDrawRect(renderer_, &srect);
}

void SDLRenderer::draw_line(Position start, Position end, Color color) {
    set_draw_color(color);
    SDL_RenderDrawLine(renderer_, static_cast<int>(start.x), static_cast<int>(start.y),
                       static_cast<int>(end.x), static_cast<int>(end.y));
}

void SDLRenderer::draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                               double angle) {
    const auto it = textures_.find(id);
    if (it == textures_.end()) return;  // 未登録 ID は無視（必要ならエラーを返す API を追加）

    SDL_Texture* tex = it->second;
    const SDL_Rect src = to_sdl_rect(src_region);
    const SDL_Rect dst = to_sdl_rect(dst_region);

    SDL_RenderCopyEx(renderer_, tex, &src, &dst, angle, nullptr, SDL_FLIP_NONE);
}

// ──────────── テクスチャ管理 ────────────
tl::expected<TextureId, std::string> SDLRenderer::register_texture(SDL_Texture* tex) {
    if (!tex) {
        return tl::unexpected<std::string>{"register_texture: texture is null"};
    }
    const TextureId id = next_id_++;
    textures_.emplace(id, tex);
    return id;
}

// ──────────── フォント管理 ────────────
tl::expected<FontId, std::string> SDLRenderer::register_font(const std::string& path, int pt_size) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), pt_size);
    if (!font) {
        return tl::unexpected<std::string>(std::string{"TTF_OpenFont failed: "} + TTF_GetError());
    }
    const FontId id = next_font_id_++;
    fonts_.emplace(id, font);
    return id;
}

// ──────────── テキスト描画 ────────────
tl::expected<void, std::string> SDLRenderer::draw_text(FontId font_id, const std::string& utf8,
                                                       Position pos, Color color) {
    const auto it = fonts_.find(font_id);
    if (it == fonts_.end()) {
        return tl::unexpected<std::string>{"draw_text: invalid font_id"};
    }

    const SDL_Color fg{color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(it->second, utf8.c_str(), fg);
    if (!surface) {
        return tl::unexpected<std::string>(std::string{"TTF_RenderUTF8_Blended failed: "} +
                                           TTF_GetError());
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        return tl::unexpected<std::string>(std::string{"SDL_CreateTextureFromSurface failed: "} +
                                           SDL_GetError());
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);  // ★ 追加
    // 描画先矩形
    int w, h;
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
    const SDL_Rect dst{static_cast<int>(pos.x), static_cast<int>(pos.y), w, h};

    SDL_RenderCopy(renderer_, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);  // キャッシュ不要なら即破棄

    return {};
}
