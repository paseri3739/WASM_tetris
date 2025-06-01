// TitleScene.hpp
#ifndef A65EE3A1_1126_4263_9557_3A178058360A
#define A65EE3A1_1126_4263_9557_3A178058360A

#include <core/IGameState.hpp>
#include <core/scene/IScene.hpp>
#include <core/scene/NextScene.hpp>
#include <memory>
#include <optional>
#include <string>

class TitleSceneState final : public IGameState {
   public:
    TitleSceneState(int width, int height);
    TitleSceneState(int width, int height, bool transition_flag);
    ~TitleSceneState() override;

    /**
     * 入力と時間経過に基づいて次の状態を生成する
     */
    std::shared_ptr<const IGameState> step(const Input& input, double delta_time) const override;

    /// 現在の状態を描画
    void render(IRenderer& renderer) const override;

    /// シーン遷移の準備が整ったかを判定
    bool is_ready_to_transition() const noexcept override;

    void set_transition_flag(bool flag);

   private:
    // タイトル画面の状態に必要なデータをここに追加
    int width_;
    int height_;
    std::string title_text_ = "ТЕТРИС";
    std::string start_text_ = "Press ENTER to Start";
    std::string symbol_ = "☭";
    bool transition_flag_ = false;  // 遷移準備が整ったかどうか
};

class TitleScene final : public IScene {
   public:
    ~TitleScene() = default;

    // シーンの初期化が必要ならここで行う
    void initialize(const GameConfig& config) override;

    // シーンの更新処理
    void update(const double delta_time) override;

    // シーンの入力処理
    void process_input(const Input& input) override;

    // シーンの描画処理
    void render(IRenderer& renderer) override;

    // シーンの終了処理
    void cleanup() override;

    std::optional<std::unique_ptr<IScene>> take_scene_transition() override;

   private:
    std::shared_ptr<const Input> input_;
    std::shared_ptr<const IGameState> current_state_;
    std::unique_ptr<IScene> pending_scene_;
};

#endif /* A65EE3A1_1126_4263_9557_3A178058360A */
