#include <core/scene/InitialScene.hpp>

void InitialScene::initialize() {
    // 初期シーンの初期化処理
    // ここでは、ゲーム設定や初期位置などを設定する
    position_ = {100, 100};  // 初期位置を設定
    // 他の初期化処理があればここに追加
}

void InitialScene::update(const double delta_time) {
    // 初期シーンの更新処理
}

void InitialScene::process_input(const Input& input) {
    // 右
    auto right = input.key_states.find(InputKey::RIGHT);
    if (right != input.key_states.end() && right->second.is_pressed) {
        position_.x += 10;
    }
    // 左
    auto left = input.key_states.find(InputKey::LEFT);
    if (left != input.key_states.end() && left->second.is_pressed) {
        position_.x -= 10;
    }
    // 上
    auto up = input.key_states.find(InputKey::UP);
    if (up != input.key_states.end() && up->second.is_pressed) {
        position_.y -= 10;
    }
    // 下
    auto down = input.key_states.find(InputKey::DOWN);
    if (down != input.key_states.end() && down->second.is_pressed) {
        position_.y += 10;
    }
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
