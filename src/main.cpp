#include <SDL2/SDL.h>
#include <emscripten.h>
#include <core/Game.hpp>
#include <core/GameConfig.hpp>
#include <core/scene/InitialScene.hpp>
#include <core/scene/SceneManager.hpp>
#include <iostream>
#include <memory>
#include <sdl/SDLRenderer.hpp>

SDL_Surface *screenSurface = nullptr;
SDL_Window *window = nullptr;
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Fallback Window", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 640, 480,
                                          SDL_WINDOW_SHOWN  // 必要に応じて SDL_WINDOW_OPENGL も付与
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
        return 1;
    }

    // ── ここで SDL_GetWindowSurface(window) を呼ばない ──

    auto renderer_result = SDLRenderer::create(
        window,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // 例: 明示的にフラグ指定
    if (!renderer_result) {
        std::cerr << renderer_result.error() << '\n';
        return 1;
    }
    std::unique_ptr<SDLRenderer> renderer = std::move(renderer_result.value());

    auto scene_manager = std::make_unique<SceneManager>(std::make_unique<InitialScene>());
    Game game{game_config::defaultGameConfig, std::move(scene_manager), std::move(renderer)};
    game.runLoop();
    return 0;
}
