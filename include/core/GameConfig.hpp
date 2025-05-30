#ifndef A3705902_55D3_4E4F_B7C4_0899B5406266
#define A3705902_55D3_4E4F_B7C4_0899B5406266

/**
 * GameConfig.hpp
 * ゲームの設定を定義するヘッダーファイル
 * テトリスのゲーム設定を格納する構造体を定義
 */
struct GameConfig {
    struct {
        double width;
        double height;
    } window;
    struct {
        double size;
    } cell;  // square
    struct {
        int rows;
        int columns;
    } grid;
    struct {
        int frame_rate;
    } frame_rate;
};

namespace game_config {
// ゲームの設定を格納する構造体
constexpr GameConfig defaultGameConfig = {
    .window = {300, 600}, .cell = {30}, .grid = {20, 10}, .frame_rate = {60}};
}  // namespace game_config

#endif /* A3705902_55D3_4E4F_B7C4_0899B5406266 */
