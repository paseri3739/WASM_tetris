#include <core/Scene.hpp>

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
};
