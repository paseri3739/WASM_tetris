# cmake/wasm.cmake
# EMSCRIPTEN (= WebAssembly) ビルド時のみ呼び出される前提
# 複数ターゲットに再利用できるよう関数化しておく

function(setup_wasm_target TARGET_NAME)
    if(NOT EMSCRIPTEN)
        message(FATAL_ERROR "setup_wasm_target() は EMSCRIPTEN ビルドでのみ呼び出してください")
    endif()

    # 出力を *.html (wasm + shell) にする
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".html")

    # SDL2 のヘッダーを sysroot から直接参照
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SYSROOT}/include/SDL2
    )

    # Emscripten 向けリンクオプション
    target_link_options(${TARGET_NAME} PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=0"
        "-sUSE_SDL_TTF=0"
        "-sUSE_SDL_MIXER=0"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
        "-sASYNCIFY"
        "-sNO_EXIT_RUNTIME=1"
        # 必要に応じて EXPORTED_FUNCTIONS を追加
        "-sEXPORTED_FUNCTIONS=['_main']"
    )
endfunction()
