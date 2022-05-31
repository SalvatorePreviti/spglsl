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
    // if (!fpath.includes("island")) {
    //  continue; // DEBUG XXX
    // }
    const sourceCode = fs.readFileSync(fpath, "utf8");
    const extension = path.extname(fpath);
    const shaderType = extension.slice(1) as "vert" | "frag";

    let expectedSize: number | undefined;
    let expectedGzipSize: number | undefined;
    for (const line of sourceCode.split("\n")) {
      if (line.startsWith("// #expected-size:")) {
        [expectedSize, expectedGzipSize] = line
          .slice("// #expected-size:".length)
          .split(" ")
          .filter(Boolean)
          .map((x) => Number.parseInt(x));
      }
    }

    result.push({
      name: path.relative(rootFolder, fpath),
      shaderType,
      sourceCode,
      expectedSize,
      expectedGzipSize,
      hasIncludes: sourceCode.indexOf("#include") >= 0,
    });
  }
  return result;
}
