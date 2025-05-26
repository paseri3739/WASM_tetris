#ifndef B2833B7C_0978_42EE_A754_673FBA7514B8
#define B2833B7C_0978_42EE_A754_673FBA7514B8

#include <core/Input.hpp>
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
    virtual void update(const double delta_time) = 0;

    // シーンの入力処理
    virtual void process_input(const Input& input) = 0;

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
    explicit SceneManager(std::unique_ptr<Scene> initial_scene)
        : current_scene_{std::move(initial_scene)} {
        current_scene_->initialize();
    }

    void update(const double delta_time);
    void render();

    void change_scene(const std::unique_ptr<Scene> next);
    Scene& get_current() const;

   private:
    std::unique_ptr<Scene> current_scene_;
    std::unique_ptr<Scene> next_scene_;

    void apply_scene_change();
};

#endif /* B2833B7C_0978_42EE_A754_673FBA7514B8 */
