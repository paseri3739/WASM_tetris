#ifndef CECD6737_285E_48BD_BE62_13103B0254DC
#define CECD6737_285E_48BD_BE62_13103B0254DC

#include <IO/SDLInputPoller.hpp>
#include <core/GameConfig.hpp>
#include <core/scene/IScene.hpp>
#include <core/scene/SceneManager.hpp>
#include <memory>

/**
 * Game ― ゲームのメインクラス
 * ゲーム全体の管理を行うクラスで、シーンの遷移や更新、描画を担当する。
 * ゲームの設定や入力ポーリングもここで行う。
 * ゲームの初期化、ループ処理、更新処理を提供する。
 */
class Game {
   public:
    Game(const std::shared_ptr<const GameConfig>& config,
         std::unique_ptr<SceneManager> scene_manager, std::unique_ptr<IRenderer> renderer,
         std::unique_ptr<InputPoller> input_poller)
        : config_(config),
          scene_manager_(std::move(scene_manager)),
          renderer_(std::move(renderer)),
          current_input_(std::make_shared<Input>()),
          input_poller_(std::move(input_poller)) {}

    // ゲームの初期化処理
    static bool initialize();
#ifndef __EMSCRIPTEN__
    // デスクトップ環境でのゲームループ
    void runLoop();
#endif
    void tick(double deltaTime);  // 1フレーム処理（全環境共通）

   private:
    std::shared_ptr<const GameConfig> config_;  // ゲーム設定の共有ポインタ
    std::unique_ptr<SceneManager> scene_manager_;
    std::unique_ptr<IRenderer> renderer_;  // レンダラーのユニークポインタ
    std::shared_ptr<const Input> current_input_;
    std::unique_ptr<InputPoller> input_poller_;
    double last_update_time_ = 0.0;
    // ゲームの更新処理
    void update(double delta_time) const;
    void processInput();
};

#endif /* CECD6737_285E_48BD_BE62_13103B0254DC */
