# ===== cmake/wasm.cmake =====
function(setup_wasm_target target_name)
    if(NOT EMSCRIPTEN)
        message(FATAL_ERROR "setup_wasm_target() は EMSCRIPTEN ビルド専用です")
    endif()

    # --- 1. SDL2 port を先に構築 ----------------------------------------
    add_custom_target(fetch_sdl2
        # FIXME: SDL関係の追加オプションを増やしたらここを修正してください
        # pyenvビルドのpythonだとうまくいかないかもしれないので、その場合はEMSDK_PYTHON環境変数を設定してください
        COMMAND embuilder build sdl2 sdl2_image sdl2_mixer sdl2_ttf # ← 追加
        COMMENT "Emscripten ports: building SDL2 (+image/mixer/ttf) if needed"
        VERBATIM
    )

    add_dependencies(${target_name} fetch_sdl2)

    # --- 2. SDL2 のインクルードを明示 ------------------------------------
    set(EMSCRIPTEN_SYSROOT "$ENV{EMSDK}/upstream/emscripten/cache/sysroot")
    target_include_directories(${target_name} SYSTEM BEFORE PRIVATE
        "${EMSCRIPTEN_SYSROOT}/include"
        "${EMSCRIPTEN_SYSROOT}/include/SDL2"
    )

    # --- 3. 出力とリンクオプション --------------------------------------
    set_target_properties(${target_name} PROPERTIES SUFFIX ".html")
    target_link_options(${target_name} PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_WEBGL2=1"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_MIXER=2"
        "-sUSE_SDL_TTF=2"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
        "-sASYNCIFY"
        "-sEXPORTED_FUNCTIONS=['_main']"
        "--preload-file" "${CMAKE_SOURCE_DIR}/assets@/assets" # ★ 2トークン
    )
endfunction()
