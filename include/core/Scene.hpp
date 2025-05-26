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
    void update(double delta_time);
    void render();

    // 遷移先をセット（内部で Scene に this を注入）
    void change_scene(std::unique_ptr<Scene> next);

    Scene& get_current() const;

   private:
    std::unique_ptr<Scene> current_scene_;
    std::unique_ptr<Scene> next_scene_;

    void apply_scene_change();
};

#endif /* B2833B7C_0978_42EE_A754_673FBA7514B8 */
