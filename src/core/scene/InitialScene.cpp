#include <core/scene/InitialScene.hpp>

void InitialScene::initialize() {}

void InitialScene::update(const double delta_time) {
    // 初期シーンの更新処理
}

void InitialScene::process_input(const Input& input) {
    // 初期シーンの入力処理
}

void InitialScene::render(IRenderer& renderer) {
    // 初期シーンの描画処理
    constexpr Rect rect = {{0, 0}, {640, 480}};        // ← まずはウィンドウサイズに揃える
    constexpr Color background_color{0, 0, 255, 255};  // 青色
    renderer.fill_rect(rect, background_color);        // 例: 青い背景
}

void InitialScene::cleanup() {
    // 初期シーンの終了処理
}
