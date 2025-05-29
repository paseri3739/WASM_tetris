#ifndef A36BFEA9_7C8D_4A2E_A878_B5C129D4F6D0
#define A36BFEA9_7C8D_4A2E_A878_B5C129D4F6D0
#include <SDL2/SDL.h>
#include <core/Input.hpp>
#include <optional>

inline const std::pair<SDL_Keycode, InputKey> SDL_TO_INPUT_KEY_MAP[] = {
    // 移動
    {SDLK_LEFT, InputKey::LEFT},
    {SDLK_a, InputKey::LEFT},
    {SDLK_RIGHT, InputKey::RIGHT},
    {SDLK_d, InputKey::RIGHT},
    {SDLK_UP, InputKey::UP},
    {SDLK_w, InputKey::UP},
    {SDLK_DOWN, InputKey::DOWN},
    {SDLK_s, InputKey::DOWN},

    // 回転
    {SDLK_z, InputKey::ROTATE_LEFT},
    {SDLK_LCTRL, InputKey::ROTATE_LEFT},
    {SDLK_x, InputKey::ROTATE_RIGHT},
    {SDLK_UP, InputKey::ROTATE_RIGHT},  // 二重割り当て（W/↑で回転も許容する場合）

    // ハードドロップ
    {SDLK_SPACE, InputKey::DROP},

    // ポーズ
    {SDLK_p, InputKey::PAUSE},
    {SDLK_RETURN, InputKey::PAUSE},  // Enterでもポーズ許容

    // 終了
    {SDLK_ESCAPE, InputKey::QUIT},
    {SDLK_q, InputKey::QUIT},
};

namespace KeyMapping {
inline std::optional<InputKey> to_input_key(SDL_Keycode code) {
    for (const auto& [sdl, input] : SDL_TO_INPUT_KEY_MAP) {
        if (sdl == code) return input;
    }
    return std::nullopt;
}
}  // namespace KeyMapping
#endif /* A36BFEA9_7C8D_4A2E_A878_B5C129D4F6D0 */
