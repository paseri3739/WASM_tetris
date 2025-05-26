#ifndef B2833B7C_0978_42EE_A754_673FBA7514B8
#define B2833B7C_0978_42EE_A754_673FBA7514B8

#include <memory>

/**
 * Scene ― シーンのインターフェース
 */
class Scene {
   public:
    virtual ~Scene() = default;

    // シーンの初期化
    virtual void initialize() = 0;

    // シーンの更新処理
    virtual void update(double delta_time) = 0;

    // シーンの描画処理
    virtual void render() = 0;

    // シーンの終了処理
    virtual void cleanup() = 0;
};

/**
 * SceneManager ― Stateパターンに基づきシーンを管理・遷移するクラス
 */
class SceneManager {
   public:
    SceneManager() = default;

    // 現在のシーンを更新
    void update(double delta_time);

    // 現在のシーンを描画
    void render();

    // 次のシーンへ遷移（次フレームで適用）
    void change_scene(std::unique_ptr<Scene> next);

   private:
    std::unique_ptr<Scene> current_scene_;
    std::unique_ptr<Scene> next_scene_;

    // シーン遷移を即時に適用する内部処理
    void apply_scene_change();
};

#endif /* B2833B7C_0978_42EE_A754_673FBA7514B8 */
