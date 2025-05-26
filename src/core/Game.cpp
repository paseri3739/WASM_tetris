#include <chrono>
#include <cmath>  // for std::max
#include <core/Game.hpp>
#include <thread>

void Game::update(double delta_time) { this->scene_manager_.update(delta_time); }
void Game::processInput() {
    // TODO: 抽象入力を受け取り、現在のシーンに渡す
}
void Game::runLoop() {
    using clock = std::chrono::steady_clock;

    const double target_frame_duration = 1.0 / static_cast<double>(config_.frame_rate.frame_rate);

    auto last_time = clock::now();

    while (true) {
        auto now = clock::now();
        double delta_time = std::chrono::duration<double>(now - last_time).count();
        last_time = now;

        this->processInput();
        this->update(delta_time);
        this->scene_manager_.render();

        // 経過時間より短ければスリープしてFPSを一定に保つ
        double frame_time = std::chrono::duration<double>(clock::now() - now).count();
        double sleep_time = target_frame_duration - frame_time;

        if (sleep_time > 0.0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }
}
