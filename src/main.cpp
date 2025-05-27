#include <SDL2/SDL.h>
#include <emscripten.h>
#include <fstream>
#include <iostream>
#include <monad/IO.hpp>
#include <tl/expected.hpp>

IO<tl::expected<void, std::string>> logMousePosition(const std::string &filepath) {
    return IO<tl::expected<void, std::string>>([filepath]() {
        int x, y;
        SDL_GetMouseState(&x, &y);

        std::ofstream file(filepath, std::ios::app);
        if (!file.is_open())
            return tl::expected<void, std::string>(
                tl::unexpected<std::string>("ファイルオープン失敗: " + filepath));

        file << "Mouse: (" << x << ", " << y << ")\n";
        return tl::expected<void, std::string>{};
    });
}

SDL_Surface *screenSurface = nullptr;
SDL_Window *window = nullptr;

void main_loop() {
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 255));  // 白背景

    SDL_Rect rect = {100, 100, 100, 100};
    SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));  // 赤四角

    SDL_UpdateWindowSurface(window);
}

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

    // TODO: emscriptenに依存しないメインループで書く
    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}
