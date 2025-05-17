#include <SDL2/SDL.h>
#include <iostream>
#include <emscripten.h>

struct GameState
{
    int x = 100;
    int y = 100;
    int w = 100;
    int h = 100;
};

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
GameState state;

void render(const GameState &state)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白背景
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 赤四角
    SDL_Rect rect = {state.x, state.y, state.w, state.h};
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}

void main_loop()
{
    render(state);
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("WASM SDL2 Window",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640, 480,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "CreateWindow Error: " << SDL_GetError() << "\n";
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "CreateRenderer Error: " << SDL_GetError() << "\n";
        return 1;
    }

    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}
