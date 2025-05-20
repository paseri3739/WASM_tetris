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

#endif /* A3705902_55D3_4E4F_B7C4_0899B5406266 */
