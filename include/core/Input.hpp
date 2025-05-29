#ifndef AF4443E9_E719_459C_BC56_81BE22CBDE47
#define AF4443E9_E719_459C_BC56_81BE22CBDE47
#include <unordered_map>

// TODO: 抽象入力を実装。WebではSDLとバインディングする。
enum class InputKey { UP, DOWN, LEFT, RIGHT, ROTATE_LEFT, ROTATE_RIGHT, DROP, PAUSE, QUIT };

struct InputState {
    bool is_pressed = false;   // キーが押されているか
    bool is_released = false;  // キーが離されたか
    bool is_held = false;      // キーが押され続けているか
};

struct Input {
    std::unordered_map<InputKey, InputState> key_states;

    // フレーム状態をクリアした新インスタンスを返す
    [[nodiscard]] std::shared_ptr<const Input> clear_frame_state() const {
        auto next = std::make_shared<Input>(*this);
        for (auto& [_, state] : next->key_states) {
            state.is_pressed = false;
            state.is_released = false;
        }
        return next;
    }
};

class InputPoller {
   public:
    virtual ~InputPoller() = default;
    virtual std::shared_ptr<const Input> poll(std::shared_ptr<const Input> previous_input) = 0;
};

#endif /* AF4443E9_E719_459C_BC56_81BE22CBDE47 */
