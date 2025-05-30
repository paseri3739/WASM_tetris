#ifndef B2833B7C_0978_42EE_A754_673FBA7514B8
#define B2833B7C_0978_42EE_A754_673FBA7514B8

#include <core/IGameState.hpp>
#include <core/IRenderer.hpp>
#include <core/Input.hpp>
#include <memory>

/**
 * Scene ― シーンのインターフェース
 * SceneManagerによって管理されるシーンの基本インターフェース。
 * 各シーンはこのインターフェースを実装し、ゲームの状態を管理する。
 * シーンはゲームの特定の状態を表し、描画や入力処理、更新処理を行う。
 * 具体的な状態はIGameStateを通じて管理され、IGameStateはイミュータブルに更新されることを期待する。
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

   protected:
    std::shared_ptr<const IGameState> current_state_;  // ← ポインタで保持（継承先からアクセス可）
};

#endif /* B2833B7C_0978_42EE_A754_673FBA7514B8 */
