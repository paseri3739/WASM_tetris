#ifndef ADFD1949_02B3_4216_A6EB_C0B2714665E9
#define ADFD1949_02B3_4216_A6EB_C0B2714665E9
#include <core/Tetrimino.hpp>
#include <vector>

class GameState {
   public:
    GameState() = default;
    ~GameState() = default;
}

class TetriminoTypeQueue {
   private:
    std::vector<TetriminoType> queue_;
    void initializeNextTetriminos(int count);
    TetriminoType getRandomTetriminoType();

   public:
    TetriminoType getNext();
};

class TetrisRule {
   private:
    const int dropIntervalMillis = 1000;
    double accumulatedDropTime = 0;

   public:
    Tetrimino drop_tetrimino(const Tetrimino& tetrimino, double delta_time) noexcept;
};

#endif /* ADFD1949_02B3_4216_A6EB_C0B2714665E9 */
