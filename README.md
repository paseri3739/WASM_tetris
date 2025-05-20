# 依存ライブラリ
tl/expected, SDL2(emscripten), immer

# 依存インフラ
Cmake, googletest, emscripten,

# 設計の指針
TypeScriptでtype定義した値オブジェクトはstructで定義
操作関数は戻り値を破棄することを許さないために[[nodiscard]]と値返却で書く
副作用はIOモナドを定義するかCommandパターンで一番外側に追いやる
カプセル化の必要がある場合はクラスを使う。インターフェースは純粋仮想関数と抽象クラスで実現(C++の言語仕様)
例外は禁止し、tl::expectedを使ってハンドリングに統一する。。コンストラクタはnoexceptにし、失敗はexpectedで返す。
