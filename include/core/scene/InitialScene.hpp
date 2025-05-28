#ifndef D84B2884_6930_4338_8CE4_151D458C1D5E
#define D84B2884_6930_4338_8CE4_151D458C1D5E

#include <core/scene/Scene.hpp>

class InitialScene final : public IScene {
   public:
    InitialScene() = default;

    void initialize() override;

    void update(const double delta_time) override;

    void process_input(const Input& input) override;

    void render(IRenderer& renderer) override;

    void cleanup() override;
};

#endif /* D84B2884_6930_4338_8CE4_151D458C1D5E */
