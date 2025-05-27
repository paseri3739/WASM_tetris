#ifndef A36BFEA9_7C8D_4A2E_A878_B5C129D4F6D0
#define A36BFEA9_7C8D_4A2E_A878_B5C129D4F6D0
#include <SDL2/SDL.h>
#include <core/Input.hpp>
#include <optional>

inline const std::pair<SDL_Keycode, InputKey> SDL_TO_INPUT_KEY_MAP[] = {
    {SDLK_LEFT, InputKey::LEFT},  {SDLK_RIGHT, InputKey::RIGHT},   {SDLK_UP, InputKey::UP},
    {SDLK_DOWN, InputKey::DOWN},  {SDLK_z, InputKey::ROTATE_LEFT}, {SDLK_x, InputKey::ROTATE_RIGHT},
    {SDLK_SPACE, InputKey::DROP}, {SDLK_p, InputKey::PAUSE},       {SDLK_ESCAPE, InputKey::QUIT},
};

inline std::optional<InputKey> to_input_key(SDL_Keycode code) {
    for (const auto& [sdl, input] : SDL_TO_INPUT_KEY_MAP) {
        if (sdl == code) return input;
    }
    return std::nullopt;
}
#endif /* A36BFEA9_7C8D_4A2E_A878_B5C129D4F6D0 */
