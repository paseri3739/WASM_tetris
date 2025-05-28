

#ifndef C79CAE94_BCD1_41D5_AD77_2A43EE576AB7
#define C79CAE94_BCD1_41D5_AD77_2A43EE576AB7
#include <core/scene/IScene.hpp>
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
    void render(IRenderer& renderer);
    void process_input(const Input& input);

    void change_scene(std::unique_ptr<IScene> next);  // ← const を外す
    IScene& get_current() const;

   private:
    std::unique_ptr<IScene> current_scene_;
    std::unique_ptr<IScene> next_scene_;

    void apply_scene_change();
};

#endif /* C79CAE94_BCD1_41D5_AD77_2A43EE576AB7 */
