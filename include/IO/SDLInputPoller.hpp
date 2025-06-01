#ifndef AAB054B7_A6D3_4E3E_A203_66DBAA015871
#define AAB054B7_A6D3_4E3E_A203_66DBAA015871

#include <SDL2/SDL.h>
#include <core/Input.hpp>

/**
 * SDLInputPoller ― SDL を使用した入力ポーリングクラス
 * SDL イベントをポーリングし、Input オブジェクトを生成します。
 * このクラスは InputPoller インターフェースを実装し、SDL のイベントループから入力を取得します。
 * イベントの種類に応じて InputKey の状態を更新し、Input オブジェクトを返します。
 */
class SDLInputPoller final : public InputPoller {
   public:
    std::shared_ptr<const Input> poll(std::shared_ptr<const Input> previous_input) override;
};

#endif /* AAB054B7_A6D3_4E3E_A203_66DBAA015871 */
