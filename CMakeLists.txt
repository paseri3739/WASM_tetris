cmake_minimum_required(VERSION 3.0)
project(WasmHello)

add_executable(hello main.cpp)

# 出力ファイルに HTML を使う（emscripten が .html, .js, .wasm を出力）
set_target_properties(hello PROPERTIES SUFFIX ".html")

# emscripten向けのリンクオプション
target_link_options(hello PRIVATE
  "-sEXIT_RUNTIME=1"
)
