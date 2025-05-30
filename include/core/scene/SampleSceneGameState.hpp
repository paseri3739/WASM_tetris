// core/scene/SampleSceneGameState.hpp
#ifndef F1EA53AA_727E_42B0_901D_CAB3DF235528
#define F1EA53AA_727E_42B0_901D_CAB3DF235528

#include <core/IGameState.hpp>
#include <core/IRenderer.hpp>
#include <core/Input.hpp>
#include <core/Position.hpp>
#include <memory>
#include <unordered_map>

/**
 * SampleSceneGameState ― サンプルシーン用ゲーム状態
 *
 * - プレイヤー座標（position_）
 * - 各キーのホールド時間（hold_durations_）
 * - シーン遷移フラグ（transition_flag_）
 *
 * を保持し、`step()` で純粋関数的に次状態を生成する。
 */
class SampleSceneGameState final : public IGameState {
   public:
    explicit SampleSceneGameState(Position pos = {100, 100});

    [[nodiscard]]
    std::shared_ptr<const IGameState> step(const Input& input, double delta_time) const override;

    void render(IRenderer& renderer) const override;

    bool is_ready_to_transition() const noexcept override;

    // 位置を読み出したい場合のアクセサ
    [[nodiscard]] Position position() const noexcept { return position_; }

   private:
    Position position_;
    std::unordered_map<InputKey, double> hold_durations_;
    bool transition_flag_{false};
};

#endif /* F1EA53AA_727E_42B0_901D_CAB3DF235528 */
