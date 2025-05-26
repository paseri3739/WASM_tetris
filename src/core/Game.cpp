#include <chrono>
#include <core/Game.hpp>

void Game::update(double delta_time) { this->scene_manager_.update(delta_time); }
void Game::processInput() {
    // TODO: 抽象入力を受け取り、現在のシーンに渡す
}
void Game::runLoop() {
    while (true) {
        auto now = std::chrono::system_clock::now();
        auto delta_time =
            std::chrono::duration<double>(now - std::chrono::system_clock::from_time_t(
                                                    static_cast<time_t>(last_update_time_)))
                .count();
        last_update_time_ = std::chrono::system_clock::to_time_t(now);
        this->processInput();
        this->update(delta_time);
        this->scene_manager_.render();
    }
}
