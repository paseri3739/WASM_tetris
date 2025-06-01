#ifndef DB541074_2FC2_44B0_9DF3_58C8A424B4A1
#define DB541074_2FC2_44B0_9DF3_58C8A424B4A1

#include <core/IGameState.hpp>
#include <core/Tetrimino.hpp>
#include <core/TetrisGrid.hpp>

/**
 * テトリスのゲーム状態を表すクラス
 * TetrisSceneState は、テトリスのゲーム状態を表現します。
 * このクラスは、テトリスのグリッド、現在のテトリミノ、およびゲームオーバー状態を保持します。
 */
class TetrisSceneState final : public IGameState {
   public:
    TetrisGrid grid;              // 不変データ構造
    Tetrimino current_tetrimino;  // 値保持
    bool is_game_over;

    TetrisSceneState(TetrisGrid g, Tetrimino t, bool over)
        : grid(std::move(g)), current_tetrimino(std::move(t)), is_game_over(over) {}

    std::shared_ptr<const IGameState> step(const Input& input, double delta_time) const override;
    void render(IRenderer& renderer) const override;
    bool is_ready_to_transition() const noexcept override;
};

#endif /* DB541074_2FC2_44B0_9DF3_58C8A424B4A1 */
