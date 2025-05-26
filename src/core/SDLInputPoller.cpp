#include <core/SDLInputPoller.hpp>

void SDLInputPoller::poll(Input& input) {
    input.clear_frame_state();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            input.key_states[InputKey::QUIT].is_pressed = true;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            SDL_Keycode code = event.key.keysym.sym;
            auto it = key_map_.find(code);
            if (it != key_map_.end()) {
                InputKey key = it->second;
                InputState& state = input.key_states[key];
                if (event.type == SDL_KEYDOWN) {
                    if (!state.is_held) state.is_pressed = true;
                    state.is_held = true;
                } else {
                    state.is_held = false;
                    state.is_released = true;
                }
            }
        }
    }
}
