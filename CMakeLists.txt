cmake_minimum_required(VERSION 3.16)
project(wasm_app LANGUAGES CXX)

include(FetchContent)

# ---- 共通の外部依存 ----
set(immer_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(immer_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(immer_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(immer_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)

set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  immer
  GIT_REPOSITORY https://github.com/arximboldi/immer.git
  GIT_TAG v0.8.1)
FetchContent_MakeAvailable(immer)

FetchContent_Declare(
  tl_expected
  GIT_REPOSITORY https://github.com/TartanLlama/expected.git
  GIT_TAG v1.1.0)
FetchContent_MakeAvailable(tl_expected)

# ─────────────────────────────────────────────────────────────
# 1) まず「純粋ロジック層」 src/core をライブラリ化（両ビルド共通）
# ─────────────────────────────────────────────────────────────
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS src/core/*.cpp)
add_library(core STATIC ${CORE_SOURCES})
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(core PUBLIC immer tl::expected)
set_property(TARGET core PROPERTY CXX_STANDARD 17)

# ─────────────────────────────────────────────────────────────
# 2) WASM ビルド時のみ UI/SDL を含む実行ファイルを生成
# ─────────────────────────────────────────────────────────────
if(EMSCRIPTEN)
  add_executable(wasm_app
    src/main.cpp # SDL2 を直接触る層
  )
  target_include_directories(wasm_app PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries(wasm_app PRIVATE core)

  # WASM オプション委譲
  include(cmake/wasm.cmake)
  setup_wasm_target(wasm_app) # 既存関数
  set(CMAKE_BUILD_TYPE Debug)
  string(APPEND CMAKE_CXX_FLAGS_DEBUG
    " -gsource-map --source-map-base http://localhost:8080/")
endif()

# ─────────────────────────────────────────────────────────────
# 3) 単体テスト（ネイティブ限定）
# ─────────────────────────────────────────────────────────────
enable_testing()

if(NOT EMSCRIPTEN AND BUILD_TESTING)
  add_subdirectory(test) # test/CMakeLists.txt を呼び出し
endif()
