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
  GIT_TAG v0.8.1
)
FetchContent_MakeAvailable(immer)

# Fetch tl::expected (Result型)
FetchContent_Declare(
  tl_expected
  GIT_REPOSITORY https://github.com/TartanLlama/expected.git
  GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(tl_expected)

if(NOT EMSCRIPTEN)
  FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.6
  )
  # SDL の不要オプションを先に潰す
  set(SDL_TEST OFF CACHE BOOL "" FORCE)
  set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(sdl2)

endif()

# 実行ファイルターゲット
add_executable(wasm_app
  src/main.cpp
)

# C++17 指定
set_property(TARGET wasm_app PROPERTY CXX_STANDARD 17)

# インクルードディレクトリ指定
target_include_directories(wasm_app PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

# ライブラリリンク(リンク対象に含めると勝手にインクルードされる)
if(NOT EMSCRIPTEN)
  target_link_libraries(wasm_app
    PRIVATE
    SDL2::SDL2 # もしくは SDL2::SDL2-static
  )
endif()
target_link_libraries(wasm_app PRIVATE immer tl::expected)

# ===== WASM 固有設定を外部ファイルに委譲 =====
if(EMSCRIPTEN)
  include(cmake/wasm.cmake)
  setup_wasm_target(wasm_app)
  set(CMAKE_BUILD_TYPE Debug) # -O0 相当
  set(CMAKE_CXX_FLAGS_DEBUG "-g -gsource-map \
  --source-map-base http://localhost:8080/") # 行番号 & SourceMap
endif()

# googletest の設定(ダミーの例) 最後に書く
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
