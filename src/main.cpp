#include <iostream>
#include <immer/vector.hpp>
#include <tl/expected.hpp>

// 関数：index指定でvectorから安全に取得
tl::expected<int, std::string> get_element(const immer::vector<int> &vec, std::size_t index)
{
    if (index >= vec.size())
    {
        return tl::unexpected("Index out of bounds");
    }
    return vec[index];
}

int main()
{
    // イミュータブルvectorの作成
    immer::vector<int> numbers = {10, 20, 30};
    auto new_numbers = numbers.push_back(40); // numbersは不変

    std::cout << "Original size: " << numbers.size() << "\n";
    std::cout << "New size: " << new_numbers.size() << "\n";

    // 値取得テスト
    auto result = get_element(new_numbers, 2);
    if (result)
    {
        std::cout << "Element at index 2: " << *result << "\n";
    }
    else
    {
        std::cout << "Error: " << result.error() << "\n";
    }

    return 0;
}
