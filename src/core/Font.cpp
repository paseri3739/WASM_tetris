#include <core/Font.hpp>

Font::Font(std::string path, int pt_size, FontId font_id, IRenderer& renderer)
    : renderer_(renderer), path_(std::move(path)), pt_size_(pt_size), font_id_(font_id) {}

tl::expected<std::unique_ptr<Font>, std::string> Font::create(const std::string& path, int pt_size,
                                                              IRenderer& renderer) {
    const auto result = renderer.register_font(path, pt_size);
    if (!result) {
        return tl::unexpected(result.error());
    }
    // std::make_uniqueの暗黙newではなく、明示的にnew
    // unique_ptrのコンストラクタ呼び出しはFont外部からの呼び出しとして展開されるため、privateコンストラクタを呼び出せない。
    return std::unique_ptr<Font>(new Font(path, pt_size, result.value(), renderer));
}

Font::~Font() {
    const auto result = renderer_.clear_font(font_id_);
    if (!result) {
        std::cerr << "Font clearing failed: " << result.error() << std::endl;
    }
}

tl::expected<void, std::string> Font::render(const std::string& utf8, Position pos, Color color) {
    return renderer_.draw_text(font_id_, utf8, pos, color);
}

std::pair<int, int> Font::measure_text(const std::string& utf8) {
    return renderer_.measure_text(font_id_, utf8);
}
