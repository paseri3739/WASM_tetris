
#include <core/Game.hpp>
#include <thread>
#ifndef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#include <chrono>
#include <cmath>  // for std::max
#endif

void Game::update(const double delta_time) const { this->scene_manager_->update(delta_time); }
void Game::processInput() {
    // SDLInputPoller で新しい Input を取得（不変）
    this->current_input_ = input_poller_->poll(this->current_input_);
    // シーンに渡すときは const Input& に展開
    this->scene_manager_->process_input(*this->current_input_);
}

bool Game::initialize() const {
    // ここでグローバルアセットのキャッシュなどを行うとよいはず
    this->scene_manager_->initialize(*config_, *renderer_);
    return true;
}
// ─────────────────────── 1フレーム処理 ───────────────────────
void Game::tick(const double deltaTime) {
    this->processInput();     // 入力収集
    this->update(deltaTime);  // ロジック更新

    // レンダリング処理
    renderer_->begin_frame();          // ← 任意（状態リセット用）
    renderer_->clear({0, 0, 0, 255});  // 背景を真っ黒でクリア (任意)
    this->scene_manager_->render(*renderer_);
    renderer_->end_frame();  // ← SDL_RenderPresent() が呼ばれる
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
