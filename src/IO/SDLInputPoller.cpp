#include <IO/KeyMapping.hpp>
#include <IO/SDLInputPoller.hpp>

std::shared_ptr<const Input> SDLInputPoller::poll(std::shared_ptr<const Input> previous_input) {
    auto input = std::make_shared<Input>(*previous_input);  // コピーして操作対象にする
    for (auto& [_, state] : input->key_states) {
        state.is_pressed = false;
        state.is_released = false;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            input->key_states[InputKey::QUIT].is_pressed = true;
            continue;
        }

        if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) continue;

        auto maybe_key = KeyMapping::to_input_key(event.key.keysym.sym);
        if (!maybe_key.has_value()) continue;

        InputKey key = maybe_key.value();
        InputState& state = input->key_states[key];

        if (event.type == SDL_KEYDOWN) {
            if (!state.is_held) state.is_pressed = true;
            state.is_held = true;
        } else {
            state.is_held = false;
            state.is_released = true;
        }
    }

    return input;
}
