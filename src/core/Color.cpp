#include <algorithm>
#include <cctype>
#include <core/graphics_types.hpp>
#include <iomanip>
#include <sstream>
#include <unordered_map>
Color Color::from_string(std::string color_str) {
    // 先頭・末尾の空白を除去
    auto trim = [](std::string& s) {
        auto ns = [](unsigned char c) { return !std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), ns));
        s.erase(std::find_if(s.rbegin(), s.rend(), ns).base(), s.end());
    };
    trim(color_str);

    // 小文字化
    std::transform(color_str.begin(), color_str.end(), color_str.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    // デフォルトは白
    Color color{255, 255, 255, 255};

    // ────────── #rrggbb / #rrggbbaa / #rgb / #rgba ──────────
    if (!color_str.empty() && color_str[0] == '#') {
        std::string hex = color_str.substr(1);
        if (hex.size() == 3 || hex.size() == 4) {  // #rgb / #rgba
            std::string expanded;
            for (char c : hex) {
                expanded.push_back(c);
                expanded.push_back(c);
            }
            hex = expanded;
        }
        if (hex.size() == 6) hex += "ff";  // α省略時は FF
        if (hex.size() == 8) {
            unsigned int v = 0;
            std::istringstream iss(hex);
            iss >> std::hex >> v;
            color = {static_cast<std::uint8_t>((v >> 24) & 0xFF),
                     static_cast<std::uint8_t>((v >> 16) & 0xFF),
                     static_cast<std::uint8_t>((v >> 8) & 0xFF),
                     static_cast<std::uint8_t>(v & 0xFF)};
            return color;
        }
    }

    // ────────── rgb(r,g,b) / rgba(r,g,b,a) ──────────
    auto parse_rgb_like = [&](const std::string& s, bool with_alpha) -> bool {
        const auto l = s.find('('), r = s.rfind(')');
        if (l == std::string::npos || r == std::string::npos || l >= r) return false;

        std::stringstream ss(s.substr(l + 1, r - l - 1));
        int v[4] = {0, 0, 0, 255};
        char comma;
        for (int i = 0; i < (with_alpha ? 4 : 3); ++i) {
            if (!(ss >> v[i])) return false;
            if (i < (with_alpha ? 3 : 2) && !(ss >> comma && comma == ',')) return false;
        }
        color = {static_cast<std::uint8_t>(std::clamp(v[0], 0, 255)),
                 static_cast<std::uint8_t>(std::clamp(v[1], 0, 255)),
                 static_cast<std::uint8_t>(std::clamp(v[2], 0, 255)),
                 static_cast<std::uint8_t>(std::clamp(v[3], 0, 255))};
        return true;
    };
    if (color_str.rfind("rgb(", 0) == 0 && parse_rgb_like(color_str, false)) return color;
    if (color_str.rfind("rgba(", 0) == 0 && parse_rgb_like(color_str, true)) return color;

    // ────────── 定義済みカラー名 ──────────
    static const std::unordered_map<std::string, Color> table = {
        {"black", {0, 0, 0, 255}},    {"white", {255, 255, 255, 255}},
        {"red", {255, 0, 0, 255}},    {"green", {0, 255, 0, 255}},
        {"blue", {0, 0, 255, 255}},   {"yellow", {255, 255, 0, 255}},
        {"cyan", {0, 255, 255, 255}}, {"magenta", {255, 0, 255, 255}},
        {"transparent", {0, 0, 0, 0}}};
    if (auto it = table.find(color_str); it != table.end()) {
        return it->second;
    }

    // 解析失敗時は既定値（白）
    return color;
}
