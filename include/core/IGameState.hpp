#ifndef B8C43484_0D94_42BD_87ED_2A239AE61EDB
#define B8C43484_0D94_42BD_87ED_2A239AE61EDB

#include <core/Input.hpp>

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
