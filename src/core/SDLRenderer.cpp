#include <core/Renderer.hpp>
class SDLRenderer : public RendererInterface {
   public:
    explicit SDLRenderer(SDL_Renderer* renderer) : sdl_renderer_{renderer} {}

    void draw_rect(int x, int y, int w, int h, SDL_Color color) override {
        SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(sdl_renderer_, &rect);
    }

   private:
    SDL_Renderer* sdl_renderer_;
};
