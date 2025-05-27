
#include <IO/Renderer.hpp>
class DrawRectCommand : public IRenderCommand {
   public:
    DrawRectCommand(int x, int y, int w, int h, SDL_Color color)
        : x_{x}, y_{y}, w_{w}, h_{h}, color_{color} {}

    void execute(RendererInterface& renderer) const override {
        renderer.draw_rect(x_, y_, w_, h_, color_);
    }

   private:
    int x_, y_, w_, h_;
    SDL_Color color_;
};
