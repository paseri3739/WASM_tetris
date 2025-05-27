#include <IO/Renderer.hpp>
#include <memory>
#include <vector>
class RenderQueue {
   public:
    void push(std::unique_ptr<IRenderCommand> cmd) { commands_.emplace_back(std::move(cmd)); }

    void execute_all(IRenderer& renderer) {
        for (const auto& cmd : commands_) {
            cmd->execute(renderer);
        }
        commands_.clear();
    }

   private:
    // このクラスがライフタイム管理するので、unique_ptrを使用
    std::vector<std::unique_ptr<IRenderCommand>> commands_;
};
