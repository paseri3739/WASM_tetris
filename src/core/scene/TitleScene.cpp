// TitleScene.cpp
#include <core/Font.hpp>
#include <core/graphics_types.hpp>  // Color など
#include <core/scene/NextScene.hpp>
#include <core/scene/TitleScene.hpp>
#include <iostream>

// --- TitleSceneState の実装 ----------------------------------------

TitleSceneState::TitleSceneState(int width, int height, Font& font)
    : width_(width), height_(height), font_(font), transition_flag_(false) {}

TitleSceneState::TitleSceneState(int width, int height, bool transition_flag, Font& font)
    : width_(width), height_(height), transition_flag_(transition_flag), font_(font) {}

TitleSceneState::~TitleSceneState() = default;

std::unique_ptr<const IGameState> TitleSceneState::step(const Input& input,
                                                        double /*delta_time*/) const {
    // タイトル画面では特に状態遷移は行わない
    // ただし、ENTERキーが押された場合は遷移準備を行う
    if (input.key_states.count(InputKey::PAUSE) &&
        input.key_states.at(InputKey::PAUSE).is_pressed) {
        return std::make_unique<TitleSceneState>(width_, height_, true, font_);  // TO DO:遷移準備
    }
    return std::make_unique<TitleSceneState>(width_, height_, font_);  // 現在の状態を維持
}

void TitleSceneState::render(IRenderer& renderer) const {
    renderer.fill_rect({0, 0, static_cast<double>(width_), static_cast<double>(height_)},
                       Color::from_string("#CC0000"));

    const auto font_position = font_.measure_text(title_text_);
    const double text_x = (width_ - font_position.first) / 2.0;
    const auto result = font_.render(title_text_, {text_x, 100}, Color::from_string("#FAD202"));

    if (!result) {
        std::cerr << "Failed to draw text: " << result.error() << std::endl;
    }

    const auto result_symbol = font_.render(symbol_, {20, 50}, Color::from_string("#FAD202"));
    if (!result_symbol) {
        std::cerr << "Failed to draw symbol: " << result_symbol.error() << std::endl;
    }
}

bool TitleSceneState::is_ready_to_transition() const noexcept { return transition_flag_; }

void TitleSceneState::set_transition_flag(bool flag) { transition_flag_ = flag; }

// --- TitleScene の実装 ----------------------------------------

void TitleScene::initialize(const GameConfig& config, IRenderer& renderer) {
    const std::string font_path = "assets/Noto_Sans_JP/static/NotoSansJP-Regular.ttf";
    const int font_size = 40;
    auto result = Font::create(font_path, font_size, renderer);
    if (!result) {
        std::cerr << "Failed to create font: " << result.error() << std::endl;
        return;  // フォントのロードに失敗した場合は初期化を中止
    }
    font_ = std::move(result.value());
    // 状態の初期化：画面サイズとフォントを渡してタイトル状態を生成
    current_state_ =
        std::make_unique<TitleSceneState>(config.window.width, config.window.height, *font_);
}

void TitleScene::update(const double delta_time) {
    if (current_state_) {
        current_state_ = current_state_->step(*input_, delta_time);
    }
    if (current_state_ && current_state_->is_ready_to_transition()) {
        // 遷移準備が整った場合、次のシーンを設定
        pending_scene_ = std::make_unique<NextScene>();
    }
}

void TitleScene::process_input(const Input& input) {
    input_ = std::make_shared<Input>(input);  // immutable input 記録
}

void TitleScene::render(IRenderer& renderer) {
    if (current_state_) {
        current_state_->render(renderer);
    }
}

void TitleScene::cleanup() {}

std::optional<std::unique_ptr<IScene>> TitleScene::take_scene_transition() {
    if (pending_scene_) {
        // 遷移要求があればそれを返す
        return std::move(pending_scene_);
    }
    // 遷移しない場合は空のオプションを返す
    return std::nullopt;
}
