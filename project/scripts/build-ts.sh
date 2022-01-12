#!/bin/bash -e

npx tsc -p ./packages/spglsl/tsconfig.json

npx eslint --no-ignore --fix ./packages/spglsl/dist
npx prettier --ignore-unknown --write ./packages/spglsl/dist/*.js ./packages/spglsl/dist/*.ts
