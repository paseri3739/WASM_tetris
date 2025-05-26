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
    std::unordered_map<InputKey, InputState> key_states;  // キー状態のマップ
    void clear_frame_state();
};

class InputPoller {
   public:
    virtual ~InputPoller() = default;
    virtual void poll(Input& input) = 0;  // 入力状態をポーリングして更新
};

#endif /* AF4443E9_E719_459C_BC56_81BE22CBDE47 */
