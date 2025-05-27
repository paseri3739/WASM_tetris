#ifndef AAB054B7_A6D3_4E3E_A203_66DBAA015871
#define AAB054B7_A6D3_4E3E_A203_66DBAA015871

#include <SDL2/SDL.h>
#include <core/Input.hpp>

class SDLInputPoller : public InputPoller {
   public:
    void poll(Input& input) override;

   private:
    // SDLのキーコードとInputKeyのマッピング。TODO: ここに必要なキーを追加していく。
    std::unordered_map<SDL_Keycode, InputKey> key_map_ = {
        {SDLK_LEFT, InputKey::LEFT}, {SDLK_RIGHT, InputKey::RIGHT}, {SDLK_UP, InputKey::UP},
        {SDLK_DOWN, InputKey::DOWN}, {SDLK_ESCAPE, InputKey::QUIT},
    };
};

#endif /* AAB054B7_A6D3_4E3E_A203_66DBAA015871 */
