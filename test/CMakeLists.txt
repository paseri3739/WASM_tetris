# GoogleTest を使った単体テスト設定（wasm_app 本体とは独立）

# ./test 以下の *.cpp を再帰的に収集
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(all_tests ${TEST_SOURCES})

# 本体のヘッダーを参照できるようにする
target_include_directories(all_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include)

set_property(TARGET all_tests PROPERTY CXX_STANDARD 17)

# core ライブラリとリンク
target_link_libraries(all_tests
    PRIVATE
    core # ← 追加
    immer tl::expected # core と同じ依存を横流し
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(all_tests)
