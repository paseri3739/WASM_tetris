#ifndef B8C43484_0D94_42BD_87ED_2A239AE61EDB
#define B8C43484_0D94_42BD_87ED_2A239AE61EDB

#include <core/Input.hpp>

/**
 * IGameState ― ゲーム状態のインターフェース
 * ゲームの状態を表現するインターフェースで、状態遷移を行うためのメソッドを定義します。
 * 各ゲーム状態はこのインターフェースを実装し、入力と時間経過に基づいて次の状態を生成します。
 * このインターフェースは、ゲームの状態遷移を関数型で行うことを期待しています。
 */
class IGameState {
   public:
    virtual ~IGameState() {}
    /**
     * 入力と時間経過に基づいて次の状態を生成する
     */
    [[nodiscard]]
    virtual std::shared_ptr<IGameState> step(const Input& input, double delta_time) const = 0;
};

#endif /* B8C43484_0D94_42BD_87ED_2A239AE61EDB */
