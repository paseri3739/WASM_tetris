#ifndef B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#define B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#include <string>
class IRenderer {
   public:
    virtual ~IRenderer() = default;

    virtual void draw_rect(int x, int y, int w, int h, std::string color) = 0;
    // 他にも draw_texture, draw_text など必要に応じて追加
};

class IRenderCommand {
   public:
    virtual ~IRenderCommand() = default;
    virtual void execute(IRenderer& renderer) const = 0;
};

#endif /* B8AC84C6_6A5A_4990_8095_F03C1115A0EC */
