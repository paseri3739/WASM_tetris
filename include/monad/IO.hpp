#ifndef B1245E63_B187_47E7_97D2_5299D4F3693A
#define B1245E63_B187_47E7_97D2_5299D4F3693A

#include <functional>
template <typename T>
/**
 * IO モナド
 * IO モナドは副作用を伴う計算を表現するためのモナドです。
 * ここでは、関数オブジェクトを使って副作用のある計算を遅延評価し、結果を取得するためのメソッドを提供します。
 * IO モナドは、関数型プログラミングのスタイルで副作用を管理するために使用されます。
 * 例えば、ファイルの読み書きやユーザー入力の取得など、外部とのやり取りを行う際に利用されます。
 */
class IO {
   public:
    // コンストラクタ 引数1つのコンストラクタにはexplicitをつける
    explicit IO(std::function<T()> action) : action_(std::move(action)) {}

    // action_は () -> T の関数オブジェクトなので実行するとT型の値を返す
    T run() const { return action_(); }

    template <typename F>
    // auto f() -> 型(後置) (trailing return type)
    // decltypeの中でdeclvalを使うことで、fの引数の型を取得(typeofのようなもの)
    // declval<T>()は、T型の値を生成せずにT型の値を表現するためのもの
    // (型がメソッドを持っているならこの後にdeclval<T>().メソッド名()と書くことでメソッドの戻り値の型を取得できる)
    auto map(F f) const -> IO<decltype(f(std::declval<T>()))> {
        return IO<decltype(f(std::declval<T>()))>([=] { return f(action_()); });
    }

    template <typename F>
    auto flatMap(F f) const -> decltype(f(std::declval<T>())) {
        return f(action_());
    }

   private:
    std::function<T()> action_;  // () -> T の関数オブジェクト
};

#endif /* B1245E63_B187_47E7_97D2_5299D4F3693A */
