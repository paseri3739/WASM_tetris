// core/scene/SampleSceneGameState.cpp
#include <cmath>
#include <core/graphics_types.hpp>  // Color, Rect など
#include <core/scene/SampleSceneGameState.hpp>
#include <iostream>
#include <utility>  // std::move

// ─────────────────────────────────────────────
// コンストラクタ
// ─────────────────────────────────────────────
SampleSceneGameState::SampleSceneGameState(Position pos) : position_{pos} {
    // すべてのキーのホールド時間を 0 初期化
    for (auto key : {InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN}) {
        hold_durations_[key] = 0.0;
    }
}

// ─────────────────────────────────────────────
// 状態遷移 (純粋関数)
// ─────────────────────────────────────────────
std::shared_ptr<const IGameState> SampleSceneGameState::step(const Input& input, double dt) const {
    constexpr double repeat_delay = 0.30;  // 最初に動き出すまでの遅延
    constexpr double repeat_rate = 0.10;   // リピート間隔
    constexpr double step_px = 10.0;       // 1 ステップで動く距離 [px]

    // コピーして新しい値オブジェクトを作る
    SampleSceneGameState next = *this;

    // ── 入力キーごとの処理 ─────────────────────
    for (auto key : {InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN}) {
        const auto it = input.key_states.find(key);

        if (it == input.key_states.end()) {  // キー情報がない
            next.hold_durations_[key] = 0.0;
            continue;
        }

        const auto& st = it->second;

        if (st.is_held || st.is_pressed) {
            double new_duration = next.hold_durations_[key] + dt;

            bool should_move =
                (st.is_pressed && new_duration >= 0.0) ||  // 押した瞬間
                (new_duration >= repeat_delay &&
                 std::fmod(new_duration - repeat_delay, repeat_rate) < dt);  // オートリピート

            if (should_move) {
                switch (key) {
                    case InputKey::LEFT:
                        next.position_.x -= step_px;
                        break;
                    case InputKey::RIGHT:
                        next.position_.x += step_px;
                        break;
                    case InputKey::UP:
                        next.position_.y -= step_px;
                        break;
                    case InputKey::DOWN:
                        next.position_.y += step_px;
                        break;
                    default:
                        break;
                }
            }
            next.hold_durations_[key] = new_duration;

        } else {
            next.hold_durations_[key] = 0.0;
        }
    }

    // ── シーン遷移判定の例 ───────────────────────
    // 例：プレイヤーが x > 500 に達したら遷移フラグを立てる
    if (next.position_.x > 500) {
        next.transition_flag_ = true;
    }

    return std::make_shared<SampleSceneGameState>(std::move(next));
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
