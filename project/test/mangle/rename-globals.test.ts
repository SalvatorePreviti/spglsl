import path from "path";
import fs from "fs";
import { spglslAngleCompile, spglslPreload } from "spglsl";
import { expect } from "chai";

describe("rename-globals", function () {
  this.timeout(7000);

  before(async () => {
    await spglslPreload();
  });

  it(`renames globals`, async () => {
    const compiled = await spglslAngleCompile({
      compileMode: "Optimize",
      language: "frag",
      beautify: false,
      minify: true,
      mangle: true,
      mangle_global_map: {
        uniformA: "a",
        uniformB: "b",
        uniformC: "c",
        inputA: "t",
        inputB: "y",
        inputC: "u",
        outputA: "T",
        outputB: "Y",
        outputC: "U",
      },
      mainSourceCode: await fs.promises.readFile(path.resolve(__dirname, "rename-globals.frag"), "utf8"),
    });

    const info = `${compiled.compileMode} ${compiled.language} ${compiled.outputVersion}\n${compiled.infoLog.toString({
      colors: true,
    })}`;
    expect(compiled.infoLog.getCounts().errors).to.equal(0, info);

    console.log(compiled.output);

    expect(compiled.valid).to.equal(true);
    expect(compiled.output).to.not.be.null;

    expect(compiled.uniforms).to.deep.equal({
      uniformA: "a",
      uniformB: "b",
      uniformC: "c",
    });

    expect(compiled.globals).to.deep.equal({
      inputA: "t",
      inputB: "y",
      inputC: "u",
      outputA: "T",
      outputB: "Y",
      outputC: "U",
    });

    expect(compiled.output).to.equal(
      `#version 300 es\nprecision highp float;uniform vec2 a,b,c;in vec2 t,y,u;layout(location=0)out vec2 T;layout(location=1)out vec2 Y;layout(location=2)out vec2 U;void main(){T=t*a,Y=y*b,U=u*c;}`,
    );

    const validated = await spglslAngleCompile({
      compileMode: "Validate",
      mainSourceCode: compiled.output || "",
    });

    expect(validated.valid).to.equal(true);

    expect(validated.uniforms).to.deep.equal({
      a: "a",
      b: "b",
      c: "c",
    });

    expect(validated.globals).to.deep.equal({
      t: "t",
      y: "y",
      u: "u",
      T: "T",
      Y: "Y",
      U: "U",
    });
  });
});
