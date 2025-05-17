cmake_minimum_required(VERSION 3.16)
project(wasm_app LANGUAGES CXX)

include(FetchContent)

# Immer の不要オプションを無効化（小文字に注意）
set(immer_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(immer_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(immer_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(immer_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)



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
)

# ライブラリリンク
target_link_libraries(wasm_app PRIVATE immer)

# C++17 指定
set_property(TARGET wasm_app PROPERTY CXX_STANDARD 17)

# WASM 向け出力用の設定
if(EMSCRIPTEN)
  set_target_properties(wasm_app PROPERTIES SUFFIX ".html")

  # SDL2のincludeをターゲットに明示的に追加
  target_include_directories(wasm_app PRIVATE
    ${CMAKE_SYSROOT}/include/SDL2
  )

  target_link_options(wasm_app PRIVATE
    "-sUSE_SDL=2"
    "-sUSE_SDL_IMAGE=0"
    "-sUSE_SDL_TTF=0"
    "-sUSE_SDL_MIXER=0"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sFULL_ES3=1"
    "-sASYNCIFY"
    "-sNO_EXIT_RUNTIME=1"
    "-sEXPORTED_FUNCTIONS=['_main']"
  )
endif()
