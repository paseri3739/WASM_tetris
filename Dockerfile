# Dockerfile
FROM emscripten/emsdk:latest

WORKDIR /app

# ソースコードをコピー
COPY . .

# ビルドコマンド（任意に書き換えてください）
RUN emcc main.cpp -o index.html \
    -s WASM=1 \
    -s EXPORTED_FUNCTIONS="['_main']" \
    -s EXPORTED_RUNTIME_METHODS="['cwrap', 'ccall']"
