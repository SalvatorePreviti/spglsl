#!/bin/bash -e
npx mocha --recursive --require @swc-node/register test/*.test.ts test/**/*.test.ts
