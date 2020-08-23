#!/bin/bash -e
source ../emsdk/emsdk_env.sh
export EMSCRIPTEN=${EMSDK}/upstream/emscripten

echo

cmake --build ../build --parallel 6

ls -lh ./packages/spglsl/wasm || true

