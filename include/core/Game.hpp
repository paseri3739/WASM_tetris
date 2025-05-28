#ifndef CECD6737_285E_48BD_BE62_13103B0254DC
#define CECD6737_285E_48BD_BE62_13103B0254DC

#include <core/GameConfig.hpp>
#include <core/scene/IScene.hpp>
#include <core/scene/SceneManager.hpp>
#include <memory>

class Game {
   public:
    explicit Game(const GameConfig& config, std::unique_ptr<SceneManager> scene_manager,
                  std::unique_ptr<IRenderer> renderer)
        : config_(config),
          scene_manager_(std::move(scene_manager)),
          renderer_(std::move(renderer)) {}

    // ゲームの設定を取得
    const GameConfig& getConfig() const { return config_; }
    // ゲームの初期化処理
    bool initialize();
    void runLoop();
    void tick(double deltaTime);  // 1フレーム処理（全環境共通）

   private:
    GameConfig config_;
    std::unique_ptr<SceneManager> scene_manager_;
    std::unique_ptr<IRenderer> renderer_;  // レンダラーのユニークポインタ
    double last_update_time_ = 0.0;
    // ゲームの更新処理
    void update(double delta_time);
    void processInput();
};

#endif /* CECD6737_285E_48BD_BE62_13103B0254DC */
