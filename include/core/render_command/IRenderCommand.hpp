#ifndef B8EC3BF1_19F1_4FCC_94AA_4BA4F20AE598
#define B8EC3BF1_19F1_4FCC_94AA_4BA4F20AE598

#include <core/Renderer.hpp>
class IRenderCommand {
   public:
    virtual ~IRenderCommand() = default;
    virtual void execute(IRenderer& renderer) const = 0;
};

#endif /* B8EC3BF1_19F1_4FCC_94AA_4BA4F20AE598 */
