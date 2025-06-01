#ifndef AF4443E9_E719_459C_BC56_81BE22CBDE47
#define AF4443E9_E719_459C_BC56_81BE22CBDE47
#include <string>
#include <unordered_map>

/**
 * 抽象キー入力を表現するenum
 */
enum class InputKey { UP, DOWN, LEFT, RIGHT, ROTATE_LEFT, ROTATE_RIGHT, DROP, PAUSE, QUIT };

/**
 * キー入力の状態を表現する構造体
 * - is_pressed: キーが押された瞬間
 * - is_released: キーが離された瞬間
 * - is_held: キーが押され続けている状態
 */
struct InputState {
    bool is_pressed = false;   // キーが押されているか
    bool is_released = false;  // キーが離されたか
    bool is_held = false;      // キーが押され続けているか
};

/**
 * 入力状態を表現する構造体
 * - key_states: 各キーの状態を保持するマップ
 * - clear_frame_state: フレーム状態をクリアした新インスタンスを返す
 * - to_string: 入力状態を文字列に変換する
 */
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

    [[nodiscard]]
    std::string to_string() const {
        std::string result;
        for (const auto& [key, state] : key_states) {
            result += "Key: " + std::to_string(static_cast<int>(key)) +
                      ", Pressed: " + std::to_string(state.is_pressed) +
                      ", Released: " + std::to_string(state.is_released) +
                      ", Held: " + std::to_string(state.is_held) + "\n";
        }
        return result;
    }
};

class InputPoller {
   public:
    virtual ~InputPoller() = default;
    virtual std::shared_ptr<const Input> poll(std::shared_ptr<const Input> previous_input) = 0;
};

#endif /* AF4443E9_E719_459C_BC56_81BE22CBDE47 */
