#include <SDL2/SDL_image.h>
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
tl::expected<std::shared_ptr<SDLRenderer>, std::string> SDLRenderer::create(SDL_Window* window,
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

    return std::shared_ptr<SDLRenderer>{new SDLRenderer{window, renderer}};
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
void SDLRenderer::draw_point(const Position& pos, Color color) {
    set_draw_color(color);
    SDL_RenderDrawPoint(renderer_, static_cast<int>(pos.x), static_cast<int>(pos.y));
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
void SDLRenderer::draw_points(const Position* points, int count, Color color) {
    set_draw_color(color);
    for (int i = 0; i < count; ++i) {
        SDL_RenderDrawPoint(renderer_, static_cast<int>(points[i].x),
                            static_cast<int>(points[i].y));
    }
}

void SDLRenderer::draw_lines(const Position* segments, int count, Color color) {
    // 描画色を設定
    set_draw_color(color);

    // segments 配列は { start0, end0, start1, end1, … } の長さ (count * 2) を想定
    for (int i = 0; i < count; ++i) {
        const Position& start = segments[i * 2];
        const Position& end = segments[i * 2 + 1];

        SDL_RenderDrawLine(renderer_, static_cast<int>(start.x), static_cast<int>(start.y),
                           static_cast<int>(end.x), static_cast<int>(end.y));
    }
}

void SDLRenderer::fill_rects(const Rect* rects, int count, Color color) {
    set_draw_color(color);
    std::vector<SDL_Rect> sdl_rects;
    sdl_rects.reserve(count);
    for (int i = 0; i < count; ++i) {
        sdl_rects.push_back(to_sdl_rect(rects[i]));
    }
    SDL_RenderFillRects(renderer_, sdl_rects.data(), count);
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
    if (font == nullptr) {
        return tl::unexpected<std::string>(std::string{"TTF_OpenFont failed: "} + TTF_GetError());
    }
    const FontId id = next_font_id_++;
    fonts_.emplace(id, font);
    return id;
}

tl::expected<FontId, std::string> SDLRenderer::clear_font(FontId id) {
    // ① 存在チェックとポインタ取得
    auto it = fonts_.find(id);
    if (it == fonts_.end()) {
        return tl::unexpected<std::string>{"clear_font: invalid font_id"};
    }
    TTF_Font* font_ptr = it->second;

    // ② フォントを閉じる
    TTF_CloseFont(font_ptr);

    // ③ マップから消す
    fonts_.erase(it);

    // 正常終了時は『元のID』を返却
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

void SDLRenderer::set_logical_size(int width, int height) {
    SDL_RenderSetLogicalSize(renderer_, width, height);
}

void SDLRenderer::set_viewport(const Rect& clip_rect) {
    SDL_Rect srect = to_sdl_rect(clip_rect);
    SDL_RenderSetViewport(renderer_, &srect);
}

void SDLRenderer::set_scale(double scale_x, double scale_y) {
    SDL_RenderSetScale(renderer_, static_cast<float>(scale_x), static_cast<float>(scale_y));
}

tl::expected<TextureId, std::string> SDLRenderer::register_texture_from_file(
    const std::string& path) {
    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf) {
        return tl::unexpected<std::string>(std::string{"IMG_Load failed: "} + IMG_GetError());
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
        return tl::unexpected<std::string>(std::string{"SDL_CreateTextureFromSurface failed: "} +
                                           SDL_GetError());
    }
    return register_texture(tex);
}

tl::expected<TextureId, std::string> SDLRenderer::register_texture_from_memory(
    int width, int height, const unsigned char* pixel_data) {
    // RGBA8 の場合
    SDL_Texture* tex = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32,
                                         SDL_TEXTUREACCESS_STATIC, width, height);
    if (!tex) {
        return tl::unexpected<std::string>(std::string{"SDL_CreateTexture failed: "} +
                                           SDL_GetError());
    }
    if (SDL_UpdateTexture(tex, nullptr, pixel_data, width * 4) != 0) {
        SDL_DestroyTexture(tex);
        return tl::unexpected<std::string>(std::string{"SDL_UpdateTexture failed: "} +
                                           SDL_GetError());
    }
    return register_texture(tex);
}
tl::expected<void, std::string> SDLRenderer::set_render_target(TextureId tex_id) {
    if (tex_id == 0) {
        // 0 を特別扱いしてウィンドウ（デフォルト）に戻す
        if (SDL_SetRenderTarget(renderer_, nullptr) != 0) {
            return tl::unexpected<std::string>(SDL_GetError());
        }
    } else {
        auto it = textures_.find(tex_id);
        if (it == textures_.end()) {
            return tl::unexpected<std::string>{"set_render_target: invalid texture id"};
        }
        if (SDL_SetRenderTarget(renderer_, it->second) != 0) {
            return tl::unexpected<std::string>(SDL_GetError());
        }
    }
    return {};
}
std::pair<int, int> SDLRenderer::measure_text(FontId font_id, const std::string& utf8) {
    auto it = fonts_.find(font_id);
    if (it == fonts_.end()) {
        return {0, 0};
    }
    int w, h;
    TTF_SizeUTF8(it->second, utf8.c_str(), &w, &h);
    return {w, h};
}
/**
 * create_text_texture の実装
 *  - TTF_RenderUTF8_Blended で SDL_Surface を作成
 *  - SDL_CreateTextureFromSurface で SDL_Texture に変換
 *  - SDL_SetTextureBlendMode でブレンドモードを有効化
 *  - register_texture で内部キャッシュに登録し、得られた TextureId を返す
 */
tl::expected<TextureId, std::string> SDLRenderer::create_text_texture(FontId font_id,
                                                                      const std::string& utf8,
                                                                      Color color) {
    // 1) フォントID の妥当性チェック
    const auto it_font = fonts_.find(font_id);
    if (it_font == fonts_.end()) {
        return tl::unexpected<std::string>("create_text_texture: invalid font_id");
    }

    // 2) TTF_RenderUTF8_Blended で文字列を Surface 化
    const SDL_Color fg{color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(it_font->second, utf8.c_str(), fg);
    if (!surface) {
        return tl::unexpected<std::string>(
            std::string("create_text_texture: TTF_RenderUTF8_Blended failed: ") + TTF_GetError());
    }

    // 3) SDL_CreateTextureFromSurface でテクスチャ化
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);  // Surface はもう不要
    if (!tex) {
        return tl::unexpected<std::string>(
            std::string("create_text_texture: SDL_CreateTextureFromSurface failed: ") +
            SDL_GetError());
    }

    // 4) ブレンドモードを有効にしておく（半透明テキスト対応）
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    // 5) register_texture で内部キャッシュに登録 → TextureId を取得
    auto register_result = register_texture(tex);
    if (!register_result) {
        // register_texture 内で SDL_DestroyTexture しているはずなので、ここでは後続不要
        return tl::unexpected<std::string>(
            std::string("create_text_texture: failed to register texture: ") +
            register_result.error());
    }

    // 6) 正常終了時は登録された TextureId を返す
    return register_result.value();
}
