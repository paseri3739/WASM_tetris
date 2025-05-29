#ifndef B8C43484_0D94_42BD_87ED_2A239AE61EDB
#define B8C43484_0D94_42BD_87ED_2A239AE61EDB

#include <core/Input.hpp>

class IGameState {
   public:
    virtual ~IGameState() {}
    [[nodiscard]]
    virtual IGameState step(const IGameState& state, const Input& input,
                            double delta_time) noexcept;
};

#endif /* B8C43484_0D94_42BD_87ED_2A239AE61EDB */
