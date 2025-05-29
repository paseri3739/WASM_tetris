#include <core/TetrisRule.hpp>

Tetrimino TetrisRule::drop_tetrimino(const Tetrimino& tetrimino, double delta_time) noexcept {
    if (tetrimino.state == TetriminoStateType::PENDING) {
        return tetrimino;
    }

    this->accumulatedDropTime += delta_time;
    if (this->accumulatedDropTime >= this->dropIntervalMillis) {
        this->accumulatedDropTime -= this->dropIntervalMillis;
        return tetrimino::move(tetrimino, 0, 1);
    }
    return tetrimino;
}
