#include <core/scene/InitialScene.hpp>

void InitialScene::initialize() {
    // 初期シーンの初期化処理
    // ここでは、ゲーム設定や初期位置などを設定する
    position_ = {100, 100};  // 初期位置を設定
    // 他の初期化処理があればここに追加
}

void InitialScene::update(const double delta_time) {
    constexpr double repeat_delay = 0.3;  // 最初のディレイ
    constexpr double repeat_rate = 0.1;   // リピート間隔

    for (auto key : {InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN}) {
        auto it = last_input_->key_states.find(key);
        if (it != last_input_->key_states.end()) {
            const auto& state = it->second;

            if (state.is_held || state.is_pressed) {
                // 時間を加算
                hold_durations_[key] += delta_time;

                // 最初の遅延と以降のリピート処理
                double duration = hold_durations_[key];
                if ((state.is_pressed && duration >= 0.0) ||  // 押された瞬間
                    (duration >= repeat_delay &&
                     fmod(duration - repeat_delay, repeat_rate) < delta_time)) {
                    // 実行タイミング
                    switch (key) {
                        case InputKey::LEFT:
                            position_.x -= 10;
                            break;
                        case InputKey::RIGHT:
                            position_.x += 10;
                            break;
                        case InputKey::UP:
                            position_.y -= 10;
                            break;
                        case InputKey::DOWN:
                            position_.y += 10;
                            break;
                        default:
                            break;
                    }
                }
            } else {
                // 押してないなら時間リセット
                hold_durations_[key] = 0.0;
            }
        }
    }
}

void InitialScene::process_input(const Input& input) {
    last_input_ = std::make_shared<Input>(input);  // ※ shallow copy で良い
}

void InitialScene::render(IRenderer& renderer) {
    // 初期シーンの描画処理
    Rect rect = {position_, {100, 200}};               // ← まずはウィンドウサイズに揃える
    constexpr Color background_color{0, 0, 255, 255};  // 青色
    renderer.fill_rect(rect, background_color);        // 例: 青い背景
}

void InitialScene::cleanup() {
    // 初期シーンの終了処理
}
