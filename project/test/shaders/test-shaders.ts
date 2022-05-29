import fs from "fs";
import path from "path";
import fastglob from "fast-glob";
import type { TestShader } from "../lib/test-shader";

let _testShaders: readonly TestShader[] | null;

export function getTestShaders(): readonly TestShader[] {
  return _testShaders || (_testShaders = _loadTestShaders());
}

const rootFolder = path.resolve(__dirname, "../..");

getTestShaders.projectRoot = rootFolder;

function _loadTestShaders(): TestShader[] {
  const allFiles = fastglob.sync(path.resolve(__dirname, "**/*.(frag|vert)"));
  const result: TestShader[] = [];
  for (const fpath of allFiles) {
    if (!fpath.includes("complicated")) {
      continue; // DEBUG XXX
    }
    const sourceCode = fs.readFileSync(fpath, "utf8");
    const extension = path.extname(fpath);
    const shaderType = extension.slice(1) as "vert" | "frag";
    result.push({
      name: path.relative(rootFolder, fpath),
      shaderType,
      sourceCode,
      hasIncludes: sourceCode.indexOf("#include") >= 0,
    });
  }
  return result;
}
