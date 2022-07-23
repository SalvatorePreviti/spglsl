import path from "path";
import fs from "fs";
import { spglslAngleCompile, spglslPreload } from "spglsl";
import { expect } from "chai";

describe("constDefs", function () {
  this.timeout(7000);

  before(async () => {
    await spglslPreload();
  });

  it(`exposes simple #define constants`, async () => {
    const compiled = await spglslAngleCompile({
      compileMode: "Optimize",
      language: "frag",
      beautify: false,
      minify: true,
      mangle: true,
      mainSourceCode: await fs.promises.readFile(path.resolve(__dirname, "const-defs.frag"), "utf8"),
    });

    const info = `${compiled.compileMode} ${compiled.language} ${compiled.outputVersion}\n${compiled.infoLog.toString({
      colors: true,
    })}`;
    expect(compiled.infoLog.getCounts().errors).to.equal(0, info);

    expect(compiled.valid).to.equal(true);
    expect(compiled.output).to.not.be.null;

    expect(compiled.constDefs).to.deep.equal({
      HELLOa: 0,
      HELLOb: 1.1,
      HELLOc: 2.3,
      HELLOd: true,
      HELLOe: false,
    });
  });
});
