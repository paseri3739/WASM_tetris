#include <core/scene/InitialScene.hpp>
#include <core/scene/SampleSceneGameState.hpp>

void InitialScene::initialize() {
    // 状態の初期化：100,100 を初期位置とする状態
    current_state_ = std::make_shared<SampleSceneGameState>(Position{100, 100});
}

void InitialScene::update(const double delta_time) {
    if (current_state_ && last_input_) {
        // 状態遷移（関数型）による更新
        current_state_ = current_state_->step(*last_input_, delta_time);
    }
}

void InitialScene::process_input(const Input& input) {
    last_input_ = std::make_shared<Input>(input);  // immutable input 記録
}

void InitialScene::render(IRenderer& renderer) {
    // 描画処理も状態に委譲
    if (auto state = std::dynamic_pointer_cast<const SampleSceneGameState>(current_state_)) {
        Rect rect = {state->position, {100, 200}};
        constexpr Color background_color{0, 0, 255, 255};
        renderer.fill_rect(rect, background_color);
    }
}

void InitialScene::cleanup() {
    // 特にリソース解放等があればここで実施
}
