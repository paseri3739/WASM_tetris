#include <core/Renderer.hpp>
#include <memory>
#include <vector>
class RenderQueue {
   public:
    void push(std::unique_ptr<IRenderCommand> cmd) { commands_.emplace_back(std::move(cmd)); }

    void execute_all(RendererInterface& renderer) {
        for (const auto& cmd : commands_) {
            cmd->execute(renderer);
        }
        commands_.clear();
    }

   private:
    std::vector<std::unique_ptr<IRenderCommand>> commands_;
};
