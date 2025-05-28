#include <chrono>
#include <cmath>  // for std::max
#include <core/Game.hpp>
#include <thread>

void Game::update(double delta_time) { this->scene_manager_->update(delta_time); }
void Game::processInput() {
    // TODO: 抽象入力を取得し、SceneManager経由で現在のシーンに渡す
    Input input = {std::unordered_map<InputKey, InputState>{
        // モック入力データ
        // TODO: 実際はSDLInputPollerを利用して入力を取得する
        {InputKey::UP, {false, false, false}},
        {InputKey::DOWN, {false, false, false}},
        {InputKey::LEFT, {false, false, false}},
        {InputKey::RIGHT, {false, false, false}},
        {InputKey::ROTATE_LEFT, {false, false, false}},
        {InputKey::ROTATE_RIGHT, {false, false, false}},
        {InputKey::DROP, {false, false, false}},
        {InputKey::PAUSE, {false, false, false}},
        {InputKey::QUIT, {false, false, false}},
    }};
    // SDLInputPollerを利用して入力を取得する処理を追加
    this->scene_manager_->process_input(input);  // シーンに入力が伝播する
}

// ─────────────────────── 1フレーム処理 ───────────────────────
void Game::tick(double deltaTime) {
    this->processInput();     // 入力収集
    this->update(deltaTime);  // ロジック更新
    this->scene_manager_->render(*renderer_);
}

// ─────────────────────── デスクトップ専用ループ ───────────────
#ifndef __EMSCRIPTEN__
void Game::runLoop() {
    using clock = std::chrono::steady_clock;

    const double target = 1.0 / static_cast<double>(config_.frame_rate.frame_rate);
    auto last = clock::now();

    while (true) {
        auto now = clock::now();
        double dt = std::chrono::duration<double>(now - last).count();
        last = now;

        tick(dt);

        // 経過時間を測って不足分だけスリープ
        double spent = std::chrono::duration<double>(clock::now() - now).count();
        double remaining = target - spent;
        if (remaining > 0.0)
            SDL_Delay(static_cast<Uint32>(remaining * 1000.0));  // 高分解能ではないが十分
    }
}
#endif
