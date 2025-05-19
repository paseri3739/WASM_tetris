#!/usr/bin/env bash
set -euo pipefail

# CPU コア数取得（macOS / Linux 両対応）
NPROC=$(command -v nproc >/dev/null 2>&1 && nproc || sysctl -n hw.ncpu)

# ===== 引数解析 ============================================================
MODE="${1:-native}"     # 引数なしなら native-test
BUILD_DIR="build/${MODE}"

# ===== 準備 ===============================================================
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# ===== CMake 生成フェーズ ==================================================
if [[ "${MODE}" == "wasm" ]]; then
  source ~/emsdk/emsdk_env.sh
  emcmake cmake -DCMAKE_BUILD_TYPE=Debug ../..
else
  cmake -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Debug ../..
fi

# ===== ビルド & テスト =====================================================
cmake --build . -j"${NPROC}"

if [[ "${MODE}" == "native" ]]; then
  ctest --output-on-failure
fi
