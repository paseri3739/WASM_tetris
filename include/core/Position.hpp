#ifndef DB5D9C5A_4147_4189_B61C_72287BB1B903
#define DB5D9C5A_4147_4189_B61C_72287BB1B903

/**
 * 位置を表す構造体
 * {x, y} 座標を持つ
 */
struct Position {
    double x;
    double y;
};

/**
 * グリッド内のセル位置を表す構造体
 * 列と行のインデックスを持つ
 * {column, row}
 */
struct GridColumnRow {
    int column;
    int row;
};

#endif /* DB5D9C5A_4147_4189_B61C_72287BB1B903 */
