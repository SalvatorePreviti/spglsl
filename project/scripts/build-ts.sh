#!/bin/bash -e

tsc -p ./packages/spglsl/tsconfig.json

eslint --no-ignore --fix ./packages/spglsl/dist
prettier --ignore-unknown --write ./packages/spglsl/dist/*.js ./packages/spglsl/dist/*.ts 
