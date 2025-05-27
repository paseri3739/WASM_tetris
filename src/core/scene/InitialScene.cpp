#include <core/scene/Scene.hpp>

class InitialScene : public Scene {
   public:
    InitialScene() = default;

    void initialize() override {}

    void update(const double delta_time) override {
        // 初期シーンの更新処理
    }

    void process_input(const Input& input) override {
        // 初期シーンの入力処理
    }

    void render(IRenderer& renderer) override {
        // 初期シーンの描画処理
        renderer.draw_rect(0, 0, 800, 600, "blue");  // 例: 青い背景
    }

    void cleanup() override {
        // 初期シーンの終了処理
    }
};
