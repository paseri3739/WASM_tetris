#ifndef B1245E63_B187_47E7_97D2_5299D4F3693A
#define B1245E63_B187_47E7_97D2_5299D4F3693A

#include <functional>
template <typename T>
class IO {
   public:
    explicit IO(std::function<T()> action) : action_(std::move(action)) {}

    T run() const { return action_(); }

    template <typename F>
    auto map(F f) const -> IO<decltype(f(std::declval<T>()))> {
        return IO<decltype(f(std::declval<T>()))>([=] { return f(action_()); });
    }

    template <typename F>
    auto flatMap(F f) const -> decltype(f(std::declval<T>())) {
        return f(action_());
    }

   private:
    std::function<T()> action_;
};

#endif /* B1245E63_B187_47E7_97D2_5299D4F3693A */
