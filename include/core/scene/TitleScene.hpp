#include <core/scene/IScene.hpp>

class TitleScene final : public IScene {
   public:
    ~TitleScene() = default;

    // シーンの初期化が必要ならここで行う
    void initialize() override;

    // シーンの更新処理
    void update(const double delta_time) override;

    // シーンの入力処理
    void process_input(const Input& input) override;

    // シーンの描画処理
    void render(IRenderer& renderer) override;

    // シーンの終了処理
    void cleanup() override;

    std::optional<std::unique_ptr<IScene>> take_scene_transition() override;
};
