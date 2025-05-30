#include <SDL2/SDL.h>
#include <emscripten.h>
#include <core/Game.hpp>
#include <core/GameConfig.hpp>
#include <core/scene/InitialScene.hpp>
#include <core/scene/SceneManager.hpp>
#include <iostream>
#include <memory>
#include <sdl/SDLRenderer.hpp>

std::unique_ptr<Game> g_game;  // グローバル保持
SDL_Window* window;            // ←重複定義を避ける

#ifdef __EMSCRIPTEN__
extern "C" void frame_cb(void* arg) {
    static double last_ms = emscripten_get_now();
    double now_ms = emscripten_get_now();
    double dt = (now_ms - last_ms) * 0.001;
    last_ms = now_ms;

    static_cast<Game*>(arg)->tick(dt);
}
#endif

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
        return 1;
    }

#ifdef __EMSCRIPTEN__
    Uint32 winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
#else
    Uint32 winFlags = SDL_WINDOW_SHOWN;
#endif
    // TODO: サイズを再考する
    window = SDL_CreateWindow("Fallback Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              game_config::defaultGameConfig.window.width,
                              game_config::defaultGameConfig.window.height, winFlags);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
        return 1;
    }

    // ── Renderer ──
    auto renderer_result =
        SDLRenderer::create(window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_result) {
        std::cerr << renderer_result.error() << '\n';
        return 1;
    }
    auto renderer = std::move(renderer_result.value());
    auto game_config = std::make_shared<const GameConfig>(game_config::defaultGameConfig);

    // ── SceneManager ──
    auto scene_manager =
        std::make_unique<SceneManager>(std::make_unique<InitialScene>(), game_config);

    // ── InputPoller ──
    auto input_poller = std::make_unique<SDLInputPoller>();

    // ── Game を生成し g_game に保持 ──
    g_game = std::make_unique<Game>(game_config, std::move(scene_manager), std::move(renderer),
                                    std::move(input_poller));

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(frame_cb, g_game.get(),
                                 0,      // RAF に従う
                                 true);  // 無限ループ化
#else
    g_game->runLoop();
#endif
    return 0;  // EMSCRIPTEN 側では通常到達しない
}
