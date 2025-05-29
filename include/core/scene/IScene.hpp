#ifndef B2833B7C_0978_42EE_A754_673FBA7514B8
#define B2833B7C_0978_42EE_A754_673FBA7514B8

#include <core/IGameState.hpp>
#include <core/IRenderer.hpp>
#include <core/Input.hpp>
#include <memory>

/**
 * Scene ― シーンのインターフェース
 */
class IScene {
   public:
    virtual ~IScene() = default;

    // シーンの初期化が必要ならここで行う
    virtual void initialize() = 0;

    // シーンの更新処理
    virtual void update(const double delta_time) = 0;

    // シーンの入力処理
    virtual void process_input(const Input& input) = 0;

    // シーンの描画処理
    virtual void render(IRenderer& renderer) = 0;

    // シーンの終了処理
    virtual void cleanup() = 0;

   private:
    IGameState current_state_;  // 現在のゲーム状態
};

#endif /* B2833B7C_0978_42EE_A754_673FBA7514B8 */
