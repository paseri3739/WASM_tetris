#include <cassert>
#include <core/scene/SceneManager.hpp>

void SceneManager::update(const double delta_time) {
    assert(current_scene_);
    current_scene_->update(delta_time);

    // 遷移要求をpullする
    if (auto opt = current_scene_->take_scene_transition(); opt && *opt) {
        change_scene(std::move(*opt));  // unique_ptr<IScene> へムーブ
    }

    // 遷移要求があれば適用。次のフレームは新シーンの描画になる。
    apply_scene_change();
}

void SceneManager::render(IRenderer& renderer) const {
    assert(current_scene_);
    current_scene_->render(renderer);
}

void SceneManager::process_input(const Input& input) const {
    assert(current_scene_);
    current_scene_->process_input(input);
}
void SceneManager::initialize(const GameConfig& config, const IRenderer& renderer) const {
    current_scene_->initialize(config, renderer);
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

    // 新シーンの初期化。関数呼び出しで渡しているのでコンストラクタで受け取らなくてOK。
    // コンストラクタの煩雑なオーバーロードを廃し、GameConfigに依存する初期化処理を閉じ込める目的がある。
    // 必要に応じてISceneはGameConfigをメンバ変数として保存できるが、非推奨。(しかし、型として明示されるため、依存関係が分かりやすくなっている)
    // TODO: IRendererを渡すように追加する。
    current_scene_->initialize(*game_config_, *renderer_);
}
