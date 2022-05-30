import { expect } from "chai";
import type { SpglslAngleCompileOptions } from "spglsl";
import { spglslAngleCompile } from "spglsl";
import chalk from "chalk";
import zlib from "node:zlib";

export interface TestShader {
  name: string;
  sourceCode: string;
  shaderType?: "vert" | "frag";
  hasIncludes?: boolean;
  expectedSize?: number;
  expectedGzipSize?: number;
}

export const makeTestShader = (shader: TestShader, options?: SpglslAngleCompileOptions) => {
  const testShader = async () => {
    const compiled = await spglslAngleCompile({
      compileMode: "Optimize",
      language: shader.shaderType,
      beautify: false,
      minify: true,
      mangle: true,
      ...options,
      mainFilePath: shader.name,
      mainSourceCode: shader.sourceCode,
    });

    const info = `${compiled.compileMode} ${compiled.language} ${compiled.outputVersion} ./${
      shader.name
    }\n${compiled.infoLog.toString({
      colors: true,
    })}`;

    expect(compiled.valid).to.equal(true, info);
    expect(compiled.infoLog.getCounts().errors).to.equal(0, info);

    console.log(chalk.magentaBright(`----- ${shader.name} -----`));
    // console.log(shader.sourceCode);
    console.log(chalk.magentaBright("---------"));
    console.log(chalk.greenBright(compiled.output !== null ? compiled.output : null));
    console.log(chalk.magentaBright("---------"));

    const validated = await spglslAngleCompile({
      compileMode: "Validate",
      mainFilePath: compiled.mainFilePath,
      mainSourceCode: compiled.output || shader.sourceCode,
      customData: compiled.customData,
      language: compiled.language,
    });

    const info1 = `${validated.compileMode} ${validated.language} ${validated.outputVersion} ./${
      shader.name
    }\n${validated.infoLog.toString({
      colors: true,
    })} ${compiled.output}`;

    expect(validated.valid).to.equal(true, info1);
    expect(validated.infoLog.getCounts().errors).to.equal(0, info);

    const out = compiled.output || "";
    const outZlibSize = zlib.gzipSync(out, { level: 9 }).length;

    console.log("output size   :", chalk.cyanBright(`${out.length} bytes (gzipped ${outZlibSize} bytes)`));

    if (shader.expectedSize) {
      let msg = `expected size : ${shader.expectedSize} bytes`;
      if (shader.expectedGzipSize) {
        msg += ` (gzipped ${shader.expectedGzipSize} bytes)`;
      }
      console.log(msg);
      if (shader.expectedGzipSize) {
        console.log(
          "difference    :",
          out.length - shader.expectedSize,
          "bytes. gzipped: ",
          outZlibSize - shader.expectedGzipSize,
          "bytes",
        );
      } else {
        console.log("difference    :", out.length - shader.expectedSize, "bytes");
      }
    }
    return compiled;
  };

  Reflect.defineProperty(testShader, "name", {
    value: `./${testShader.name}`,
    configurable: true,
    enumerable: false,
    writable: true,
  });

  return testShader;
};
