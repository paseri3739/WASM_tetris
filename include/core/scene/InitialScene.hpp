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
    std::unordered_map<InputKey, double> hold_durations_;  // 各キーの押下時間
    std::shared_ptr<const Input> last_input_;              // 最後に受け取った入力
};

#endif /* D84B2884_6930_4338_8CE4_151D458C1D5E */
