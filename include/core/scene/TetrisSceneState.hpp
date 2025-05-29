#ifndef DB541074_2FC2_44B0_9DF3_58C8A424B4A1
#define DB541074_2FC2_44B0_9DF3_58C8A424B4A1

#include <core/IGameState.hpp>
#include <core/Tetrimino.hpp>
#include <core/TetrisGrid.hpp>

class TetrisSceneState final : public IGameState {
   public:
    const TetrisGrid grid;              // ゲームのグリッド状態
    const Tetrimino current_tetrimino;  // 現在のテトリミノ
    bool is_game_over;                  // ゲームオーバー状態
    IGameState step(const IGameState& state, const Input& input,
                    double delta_time) noexcept override;
};

#endif /* DB541074_2FC2_44B0_9DF3_58C8A424B4A1 */
