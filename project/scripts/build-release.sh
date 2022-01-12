#!/bin/bash -e
source ../emsdk/emsdk_env.sh
export EMSCRIPTEN=${EMSDK}/upstream/emscripten
mkdir -p ../build

echo

npx tsc -p ./packages/spglsl/tsconfig.json

export EMSCRIPTEN_CUSTOM_CXX_FLAGS="-O3 -g0 -flto=full -DNDEBUG -s ASSERTIONS=0"
cmake \
-S./ \
-H./ \
-B../build \
-DCMAKE_TOOLCHAIN_FILE="$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake" \
-DCMAKE_BUILD_TYPE=Release \
-G "Ninja"

cmake --build ../build --parallel 6

ls -lh ./packages/spglsl/wasm || true
