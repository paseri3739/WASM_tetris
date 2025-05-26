#ifndef CECD6737_285E_48BD_BE62_13103B0254DC
#define CECD6737_285E_48BD_BE62_13103B0254DC

#include <core/GameConfig.hpp>
#include <core/Scene.hpp>

class Game {
   public:
    explicit Game(const GameConfig& config, SceneManager& scene_manager)
        : config_(config), scene_manager_(scene_manager) {}

    // ゲームの設定を取得
    const GameConfig& getConfig() const { return config_; }
    // ゲームの初期化処理
    bool initialize();
    void runLoop();

   private:
    GameConfig config_;
    SceneManager& scene_manager_;
    double last_update_time_ = 0.0;
    // ゲームの更新処理
    void update(double delta_time);
    void processInput();
};

#endif /* CECD6737_285E_48BD_BE62_13103B0254DC */
