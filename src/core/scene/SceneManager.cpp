#include <cassert>
#include <core/scene/SceneManager.hpp>

void SceneManager::update(const double delta_time) {
    assert(current_scene_);
    current_scene_->update(delta_time);

    // 遷移要求があれば適用
    apply_scene_change();
}

void SceneManager::render(IRenderer& renderer) {
    assert(current_scene_);
    current_scene_->render(renderer);
}

void SceneManager::process_input(const Input& input) {
    assert(current_scene_);
    current_scene_->process_input(input);
}

void SceneManager::change_scene(std::unique_ptr<IScene> next) {
    // nullptr の遷移要求は無視
    if (next) {
        next_scene_ = std::move(next);  // 次フレームで適用
    }
}

IScene& SceneManager::get_current() const {
    assert(current_scene_);
    return *current_scene_;
}

void SceneManager::apply_scene_change() {
    if (!next_scene_) return;  // 遷移要求なし

    // 現シーンの後処理
    current_scene_->cleanup();

    // 所有権をスワップ
    current_scene_ = std::move(next_scene_);
    next_scene_.reset();

    // 新シーンの初期化
    current_scene_->initialize();
}
