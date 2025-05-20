#ifndef A3705902_55D3_4E4F_B7C4_0899B5406266
#define A3705902_55D3_4E4F_B7C4_0899B5406266

struct GameConfig {
    struct {
        double width;
        double height;
    } window;
    struct {
        double size;
    } cell;  // square
    struct {
        int columns;
        int rows;
    } grid;
};

namespace game_config {
// ゲームの設定を格納する構造体
constexpr GameConfig defaultGameConfig = {.window = {800, 600}, .cell = {50}, .grid = {10, 10}};

}  // namespace game_config

#endif /* A3705902_55D3_4E4F_B7C4_0899B5406266 */
