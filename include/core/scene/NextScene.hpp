#ifndef ED0A3892_C7C2_4C09_A0DE_1D84D2DE5DA6
#define ED0A3892_C7C2_4C09_A0DE_1D84D2DE5DA6

#include <core/IGameState.hpp>
#include <core/scene/IScene.hpp>

class NextSceneGameState : public IGameState {
   public:
    // 状態の初期化や更新処理をここで実装
    NextSceneGameState() = default;

    // 状態の更新処理
    std::shared_ptr<const IGameState> step(const Input& input,
                                           const double delta_time) const override {
        return std::make_shared<NextSceneGameState>(*this);
    };

    // 描画処理
    void render(IRenderer& renderer) const override {
        renderer.fill_rect({0, 0, 100, 200}, {255, 0, 0, 255});
    };

    bool is_ready_to_transition() const noexcept override {
        return false;  // 次シーンへは遷移しない
    }
};

class NextScene : public IScene {
   public:
    NextScene() = default;
    // シーンの初期化が必要ならここで行う
    void initialize() override {
        // 状態の初期化
        current_state_ = std::make_shared<NextSceneGameState>();
    };

    // シーンの更新処理
    void update(const double delta_time) override {};

    // シーンの入力処理
    void process_input(const Input& input) override {};

    // シーンの描画処理
    void render(IRenderer& renderer) override { current_state_->render(renderer); };

    // シーンの終了処理
    void cleanup() override {};

    std::unique_ptr<IScene> take_scene_transition() override {
        // シーン遷移の要求があればここで処理
        return nullptr;  // 遷移しない
    };

   private:
    std::shared_ptr<const IGameState> current_state_;  // 現在の状態を保持
};

#endif /* ED0A3892_C7C2_4C09_A0DE_1D84D2DE5DA6 */
