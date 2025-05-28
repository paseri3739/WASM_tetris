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
    int n = SDL_GetNumRenderDrivers();
    std::cout << "Available render drivers: " << n << std::endl;
    for (int i = 0; i < n; ++i) {
        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(i, &info);
        std::cout << "  - " << info.name << std::endl;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("Fallback Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return 1;
    }

    screenSurface = SDL_GetWindowSurface(window);
    if (!screenSurface) {
        std::cerr << "SDL_GetWindowSurface Error: " << SDL_GetError() << "\n";
        return 1;
    }

    // // TODO: emscriptenに依存しないメインループで書く
    // emscripten_set_main_loop(main_loop, 0, 1);
    auto initial_scene = std::make_unique<InitialScene>();
    auto scene_manager = std::make_unique<SceneManager>(std::move(initial_scene));
    auto renderer_result = SDLRenderer::create(window);
    if (!renderer_result) {
        std::cerr << "Failed to create SDLRenderer: " << renderer_result.error() << '\n';
        return 1;
    }
    // std::unique_ptr<SDLRenderer> を取り出す
    std::unique_ptr<SDLRenderer> renderer = std::move(renderer_result.value());
    Game game = Game(game_config::defaultGameConfig, std::move(scene_manager), std::move(renderer));

    game.runLoop();

    return 0;
}
