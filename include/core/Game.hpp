#ifndef CECD6737_285E_48BD_BE62_13103B0254DC
#define CECD6737_285E_48BD_BE62_13103B0254DC

#include <core/GameConfig.hpp>

class Game {
   public:
    explicit Game(const GameConfig& config) : config_(config) {}

    // ゲームの設定を取得
    const GameConfig& getConfig() const { return config_; }
    // ゲームの初期化処理
    bool initialize();
    void runLoop();

   private:
    GameConfig config_;
    // ゲームの更新処理
    void update(double delta_time);
    void processInput();
};

#endif /* CECD6737_285E_48BD_BE62_13103B0254DC */
