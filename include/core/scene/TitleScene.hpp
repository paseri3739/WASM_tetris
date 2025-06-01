// FIXME: ビルドが通らないので後で直してください
#include <core/IGameState.hpp>
#include <core/scene/IScene.hpp>
#include <iostream>
#include <memory>
#include <string>

class TitleSceneState final : public IGameState {
   public:
    TitleSceneState(int width, int height) : width_(width), height_(height) {}
    ~TitleSceneState() {}
    /**
     * 入力と時間経過に基づいて次の状態を生成する
     */
    std::shared_ptr<const IGameState> step(const Input& input, double delta_time) const override {
        // タイトル画面では特に状態遷移は行わない
        // ただし、ENTERキーが押された場合は遷移準備を行う
        if (input.key_states.count(InputKey::PAUSE) &&
            input.key_states.at(InputKey::PAUSE).is_pressed) {
            return std::make_shared<TitleSceneState>(width_, height_);  // TODO:遷移準備
        }
        return std::make_shared<TitleSceneState>(width_, height_);  // 現在の状態を維持
    };

    /// 現在の状態を描画
    void render(IRenderer& renderer) const override {
        renderer.fill_rect({0, 0, static_cast<double>(width_), static_cast<double>(height_)},
                           Color::from_string("#CC0000"));

        auto font_id =
            renderer.register_font("assets/Noto_Sans_JP/static/NotoSansJP-Regular.ttf", 24);

        if (font_id) {
            const auto font_id_value = font_id.value();
            auto result = renderer.draw_text(font_id_value, title_text_, {50, 100},
                                             Color::from_string("#FAD202"));
            if (!result) {
                std::cerr << "Failed to draw text: " << result.error() << std::endl;
            }

            auto result_symbol =
                renderer.draw_text(font_id_value, symbol_, {20, 50}, Color::from_string("#FAD202"));
            if (!result_symbol) {
                std::cerr << "Failed to draw symbol: " << result_symbol.error() << std::endl;
            }
        }
    };

    /// シーン遷移の準備が整ったかを判定
    bool is_ready_to_transition() const noexcept override { return transition_flag_; };
    void set_transition_flag(bool flag) { transition_flag_ = flag; }

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
    void initialize(const GameConfig& config) override {
        // 初期状態をタイトル画面の状態に設定
        current_state_ =
            std::make_shared<TitleSceneState>(config.window.width, config.window.height);
    };

    // シーンの更新処理
    void update(const double delta_time) override {
        if (current_state_) {
            current_state_ = current_state_->step(*input_, delta_time);
        }
    };

    // シーンの入力処理
    void process_input(const Input& input) override {
        input_ = std::make_shared<const Input>(input);  // 入力を保持
        if (current_state_) {
            // 入力に基づいて状態を更新
            auto state = std::dynamic_pointer_cast<TitleSceneState>(current_state_);
            if (state && input.key_states.count(InputKey::PAUSE) &&
                input.key_states.at(InputKey::PAUSE).is_pressed) {
                state->set_transition_flag(true);  // 遷移準備を整える
            }
        }
    };

    // シーンの描画処理
    void render(IRenderer& renderer) override {
        if (current_state_) {
            current_state_->render(renderer);
        }
    }

    // シーンの終了処理
    void cleanup() override {};

    std::optional<std::unique_ptr<IScene>> take_scene_transition() override;
    std::shared_ptr<const Input> input_;
};
