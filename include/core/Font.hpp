#ifndef B031AF70_1F8F_4C8E_8633_3B95F6EF3920
#define B031AF70_1F8F_4C8E_8633_3B95F6EF3920
#include <core/IRenderer.hpp>
class Font {
   private:
    IRenderer& renderer_;
    std::string path_;
    int pt_size_;
    FontId font_id_;

    Font(std::string path, int pt_size, FontId font_id, IRenderer& renderer);
    // コピー／ムーブを禁止
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(Font&&) = delete;

   public:
    static tl::expected<std::unique_ptr<Font>, std::string> create(const std::string& path,
                                                                   int pt_size,
                                                                   IRenderer& renderer);
    ~Font();

    tl::expected<void, std::string> render(const std::string& utf8, Position pos, Color color);
    std::pair<int, int> measure_text(const std::string& utf8);
};

#endif /* B031AF70_1F8F_4C8E_8633_3B95F6EF3920 */
