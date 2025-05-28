#include <core/scene/Scene.hpp>
#include <memory>
/**
 * SceneManager ― Stateパターンに基づきシーンを管理・遷移するクラス
 */

class SceneManager {
   public:
    explicit SceneManager(std::unique_ptr<IScene> initial_scene)
        : current_scene_{std::move(initial_scene)} {
        current_scene_->initialize();
    }

    void update(const double delta_time);
    void render();

    void change_scene(const std::unique_ptr<IScene> next);
    IScene& get_current() const;

   private:
    std::unique_ptr<IScene> current_scene_;
    std::unique_ptr<IScene> next_scene_;

    void apply_scene_change();
};
