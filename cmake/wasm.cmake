# cmake/wasm.cmake
# EMSCRIPTEN (= WebAssembly) ビルド時のみ呼び出される前提
# 複数ターゲットに再利用できるよう関数化しておく

function(setup_wasm_target TARGET_NAME)
    if(NOT EMSCRIPTEN)
        message(FATAL_ERROR "setup_wasm_target() は EMSCRIPTEN ビルドでのみ呼び出してください")
    endif()

    # 出力を *.html (wasm + shell) にする
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".html")


    # Emscripten 向けリンクオプション
    target_link_options(${TARGET_NAME} PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_MIXER=2"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
        "-sASYNCIFY"
        # 必要に応じて EXPORTED_FUNCTIONS を追加
        "-sEXPORTED_FUNCTIONS=['_main']"
    )
endfunction()
