#!/bin/bash -e

npx tsc -p ./packages/spglsl/tsconfig.json

npx prettier --ignore-unknown --write ./packages/spglsl/dist/*.js ./packages/spglsl/dist/*.ts
