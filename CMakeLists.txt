cmake_minimum_required(VERSION 3.16)
project(wasm_app LANGUAGES CXX)

include(FetchContent)

# Immer の不要オプションを無効化（小文字に注意）
set(immer_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(immer_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(immer_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(immer_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)

# tl::expected のテスト無効  ←★追加
set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Fetch immer (イミュータブルデータ構造)
FetchContent_Declare(
  immer
  GIT_REPOSITORY https://github.com/arximboldi/immer.git
  GIT_TAG master
)
FetchContent_MakeAvailable(immer)

# Fetch tl::expected (Result型)
FetchContent_Declare(
  tl_expected
  GIT_REPOSITORY https://github.com/TartanLlama/expected.git
  GIT_TAG master
)
FetchContent_MakeAvailable(tl_expected)

# 実行ファイルターゲット
add_executable(wasm_app
  src/main.cpp
)

# インクルードディレクトリ指定
target_include_directories(wasm_app PRIVATE
  ${tl_expected_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include
)

# ライブラリリンク
target_link_libraries(wasm_app PRIVATE immer)

# C++17 指定
set_property(TARGET wasm_app PROPERTY CXX_STANDARD 17)

# ===== WASM 固有設定を外部ファイルに委譲 =====
if(EMSCRIPTEN)
  include(cmake/wasm.cmake)
  setup_wasm_target(wasm_app)
endif()

set(CMAKE_BUILD_TYPE Debug) # -O0 相当
set(CMAKE_CXX_FLAGS_DEBUG "-g -gsource-map \
  --source-map-base http://localhost:8080/") # 行番号 & SourceMap


# googletest の設定(ダミーの例)
enable_testing()

add_executable(
  hello_test
  ./test/hello_test.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
