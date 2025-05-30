#ifndef F1EA53AA_727E_42B0_901D_CAB3DF235528
#define F1EA53AA_727E_42B0_901D_CAB3DF235528

#include <cmath>
#include <core/IGameState.hpp>
#include <core/Position.hpp>
#include <unordered_map>

class SampleSceneGameState : public IGameState {
   public:
    Position position;
    std::unordered_map<InputKey, double> hold_durations;

    SampleSceneGameState(Position pos = {100, 100}) : position(pos) {}

    [[nodiscard]]
    std::shared_ptr<IGameState> step(const Input& input, double delta_time) const override {
        SampleSceneGameState next = *this;

        constexpr double repeat_delay = 0.3;
        constexpr double repeat_rate = 0.1;

        for (auto key : {InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN}) {
            const auto it = input.key_states.find(key);
            if (it == input.key_states.end()) {
                next.hold_durations[key] = 0.0;
                continue;
            }

            const auto& state = it->second;
            if (state.is_held || state.is_pressed) {
                double new_duration = next.hold_durations[key] + delta_time;
                bool should_move =
                    (state.is_pressed && new_duration >= 0.0) ||
                    (new_duration >= repeat_delay &&
                     std::fmod(new_duration - repeat_delay, repeat_rate) < delta_time);
                if (should_move) {
                    switch (key) {
                        case InputKey::LEFT:
                            next.position.x -= 10;
                            break;
                        case InputKey::RIGHT:
                            next.position.x += 10;
                            break;
                        case InputKey::UP:
                            next.position.y -= 10;
                            break;
                        case InputKey::DOWN:
                            next.position.y += 10;
                            break;
                        default:
                            break;
                    }
                }
                next.hold_durations[key] = new_duration;
            } else {
                next.hold_durations[key] = 0.0;
            }
        }

        return std::make_shared<SampleSceneGameState>(std::move(next));
    }
};
#endif /* F1EA53AA_727E_42B0_901D_CAB3DF235528 */
