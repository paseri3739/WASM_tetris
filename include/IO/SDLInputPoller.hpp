#ifndef AAB054B7_A6D3_4E3E_A203_66DBAA015871
#define AAB054B7_A6D3_4E3E_A203_66DBAA015871

#include <SDL2/SDL.h>
#include <core/Input.hpp>

class SDLInputPoller : public InputPoller {
   public:
    void poll(Input& input) override;
};

#endif /* AAB054B7_A6D3_4E3E_A203_66DBAA015871 */
