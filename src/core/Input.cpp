#include <core/Input.hpp>

void Input::clear_frame_state() {
    for (auto& [_, state] : key_states) {
        state.is_pressed = false;
        state.is_released = false;
    }
}
