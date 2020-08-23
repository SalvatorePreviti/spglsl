#!/bin/bash -e

tsc -p ./packages/spglsl/tsconfig.json

acuris-eslint --fix ./packages/spglsl/dist ./packages/spglsl/wasm
