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
void Game::runLoop() {
    using clock = std::chrono::steady_clock;

    const double target_frame_duration = 1.0 / static_cast<double>(config_.frame_rate.frame_rate);

    auto last_time = clock::now();

    while (true) {
        auto now = clock::now();
        double delta_time = std::chrono::duration<double>(now - last_time).count();
        last_time = now;

        this->processInput();  // TODO: 内部でSDLInputPollerを利用して入力を取得し、シーンに渡す
        this->update(delta_time);
        this->scene_manager_->render(*this->renderer_);

        // 経過時間より短ければスリープしてFPSを一定に保つ
        double frame_time = std::chrono::duration<double>(clock::now() - now).count();
        double sleep_time = target_frame_duration - frame_time;

        if (sleep_time > 0.0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }
}
