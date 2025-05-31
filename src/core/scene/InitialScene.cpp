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
    if (current_state_ && current_state_->is_ready_to_transition()) {
        // 例: manager_.change_scene(std::make_unique<NextScene>(game_config_, manager_));
        // TODO: ロジックを試すにはNextSceneの実装を行う必要がある
    }
}

void InitialScene::process_input(const Input& input) {
    last_input_ = std::make_shared<Input>(input);  // immutable input 記録
}

void InitialScene::render(IRenderer& renderer) {
    // 描画処理も状態に委譲
    current_state_->render(renderer);
}

void InitialScene::cleanup() {
    // 特にリソース解放等があればここで実施
}

std::unique_ptr<IScene> InitialScene::take_scene_transition() { return std::move(pending_scene_); }
