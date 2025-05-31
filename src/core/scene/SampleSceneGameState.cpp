#include <cmath>
#include <core/graphics_types.hpp>  // Color, Rect など
#include <core/scene/SampleSceneGameState.hpp>
#include <iostream>
#include <utility>  // std::move

// ─────────────────────────────────────────────
// コンストラクタ
// ─────────────────────────────────────────────

// step内で呼び出す想定のコンストラクタ
SampleSceneGameState::SampleSceneGameState(Position pos, immer::map<InputKey, double> durations,
                                           bool transition_flag)
    : position_{pos}, hold_durations_{std::move(durations)}, transition_flag_{transition_flag} {}

// 初期位置のみを指定するコンストラクタ。シーン開始時に使用されている。
SampleSceneGameState::SampleSceneGameState(Position pos) : position_{pos} {
    immer::map<InputKey, double> init_map;
    for (auto key : {InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN}) {
        init_map = init_map.set(key, 0.0);
    }
    hold_durations_ = init_map;
}

// ─────────────────────────────────────────────
// 状態遷移 (純粋関数)
// ─────────────────────────────────────────────
std::shared_ptr<const IGameState> SampleSceneGameState::step(const Input& input,
                                                             double delta_time) const {
    constexpr double repeat_delay = 0.30;  // 最初に動き出すまでの遅延
    constexpr double repeat_rate = 0.10;   // リピート間隔
    constexpr double step_px = 10.0;       // 1 ステップで動く距離 [px]

    // コピーして新しい値オブジェクトを作る
    auto updated_hold_durations = hold_durations_;
    Position new_position = position_;
    bool new_transition_flag = transition_flag_;

    // ── 入力キーごとの処理 ─────────────────────
    for (auto key : {InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN}) {
        const auto it = input.key_states.find(key);
        if (it == input.key_states.end()) {
            updated_hold_durations = updated_hold_durations.set(key, 0.0);  // イミュータブル更新
            continue;
        }

        const auto& st = it->second;
        double prev_duration = hold_durations_.find(key) ? *hold_durations_.find(key) : 0.0;
        double new_duration = st.is_held || st.is_pressed ? prev_duration + delta_time : 0.0;

        bool should_move = (st.is_pressed && new_duration >= 0.0) ||
                           (new_duration >= repeat_delay &&
                            std::fmod(new_duration - repeat_delay, repeat_rate) < delta_time);

        if (should_move) {
            switch (key) {
                case InputKey::LEFT:
                    new_position.x -= step_px;
                    break;
                case InputKey::RIGHT:
                    new_position.x += step_px;
                    break;
                case InputKey::UP:
                    new_position.y -= step_px;
                    break;
                case InputKey::DOWN:
                    new_position.y += step_px;
                    break;
                default:
                    break;
            }
        }

        updated_hold_durations = updated_hold_durations.set(key, new_duration);
    }

    // シーン遷移判定
    if (new_position.x > 500) {
        new_transition_flag = true;
    }

    // 更新用コンストラクタに変わる
    return std::make_shared<SampleSceneGameState>(new_position, std::move(updated_hold_durations),
                                                  new_transition_flag);
}

// ─────────────────────────────────────────────
// 描画
// ─────────────────────────────────────────────
void SampleSceneGameState::render(IRenderer& renderer) const {
    Rect rect{position_, {100, 200}};
    Color blue{0, 0, 255, 255};

    renderer.fill_rect(rect, blue);

    auto font_id = renderer.register_font("assets/Noto_Sans_JP/static/NotoSansJP-Regular.ttf", 24);

    if (font_id) {
        auto font_id_value = font_id.value();
        auto result =
            renderer.draw_text(font_id_value, "Sample Scene", {100, 100}, {255, 255, 255, 255});
        if (!result) {
            std::cerr << "Failed to draw text: " << result.error() << std::endl;
        }
    }
}

// ─────────────────────────────────────────────
// 遷移可否判定
// ─────────────────────────────────────────────
bool SampleSceneGameState::is_ready_to_transition() const noexcept { return transition_flag_; }
