#ifndef D84B2884_6930_4338_8CE4_151D458C1D5E
#define D84B2884_6930_4338_8CE4_151D458C1D5E

#include <core/GameConfig.hpp>
#include <core/scene/IScene.hpp>

class InitialScene final : public IScene {
   public:
    InitialScene(const GameConfig& game_config) : game_config_(game_config) {};

    void initialize() override;

    void update(const double delta_time) override;

    void process_input(const Input& input) override;

    void render(IRenderer& renderer) override;

    void cleanup() override;

   private:
    GameConfig game_config_;  // ゲーム設定
    Position position_;
};

#endif /* D84B2884_6930_4338_8CE4_151D458C1D5E */
