

#ifndef C79CAE94_BCD1_41D5_AD77_2A43EE576AB7
#define C79CAE94_BCD1_41D5_AD77_2A43EE576AB7
#include <core/GameConfig.hpp>
#include <core/scene/IScene.hpp>
#include <memory>

/**
 * SceneManager ― Stateパターンに基づきシーンを管理・遷移するクラス
 * ISceneのライフサイクルを管理し、シーンの更新、描画、入力処理を行う。
 */

class SceneManager {
   public:
    SceneManager(std::unique_ptr<IScene> initial_scene,
                 const std::shared_ptr<const GameConfig>& game_config)
        : current_scene_{std::move(initial_scene)}, game_config_{game_config} {
        current_scene_->initialize(*game_config);
    }

    void update(double delta_time);
    void render(IRenderer& renderer);
    void process_input(const Input& input);

    [[nodiscard]]
    IScene& get_current() const;

   private:
    std::unique_ptr<IScene> current_scene_;
    std::unique_ptr<IScene> next_scene_;
    std::shared_ptr<const GameConfig> game_config_;

    void change_scene(std::unique_ptr<IScene> next);
    void apply_scene_change();
};

#endif /* C79CAE94_BCD1_41D5_AD77_2A43EE576AB7 */
