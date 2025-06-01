#ifndef D84B2884_6930_4338_8CE4_151D458C1D5E
#define D84B2884_6930_4338_8CE4_151D458C1D5E

#include <core/GameConfig.hpp>
#include <core/scene/IScene.hpp>

/**
 * InitialScene ― 初期シーン
 * 矩形を描画するシンプルなシーンで、入力ポーリングと状態遷移を行う。
 * このシーンはゲームの初期状態を表し、ゲーム開始時に表示される。
 * 最低限のゲームループ機構を持つ。
 */
class InitialScene final : public IScene {
   public:
    void initialize(const GameConfig& config, const IRenderer& renderer) override;

    void update(double delta_time) override;

    void process_input(const Input& input) override;

    void render(IRenderer& renderer) override;

    void cleanup() override;

    std::optional<std::unique_ptr<IScene>> take_scene_transition() override;

   private:
    std::shared_ptr<const Input> last_input_;  // 入力は保持（Immutable）
};

#endif /* D84B2884_6930_4338_8CE4_151D458C1D5E */
