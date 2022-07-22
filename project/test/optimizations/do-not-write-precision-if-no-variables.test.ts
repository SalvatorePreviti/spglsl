import { expect } from "chai";
import { spglslAngleCompile, SpglslAngleCompileError } from "spglsl";

describe("do-not-write-precision-if-no-variables", function () {
  this.timeout(7000);

  it("does not write precision specifiers if there are no variables used", async () => {
    expect(await compile("#version 300 es\nvoid main(){}")).to.eq("#version 300 es\nvoid main(){}");
    expect(await compile("#version 300 es\nprecision highp float; void main(){}")).to.eq(
      "#version 300 es\nvoid main(){}",
    );
  });
});

async function compile(code: string): Promise<string> {
  const compiled = await spglslAngleCompile({
    mainSourceCode: code,
    compileMode: "Optimize",
    mangle: false,
    minify: true,
    beautify: false,
  });
  if (compiled.infoLog.hasErrors()) {
    throw new SpglslAngleCompileError(compiled);
  }
  if (compiled.output) {
    const validated = await spglslAngleCompile({
      mainSourceCode: compiled.output,
      compileMode: "Validate",
    });
    if (validated.infoLog.hasErrors()) {
      throw new SpglslAngleCompileError(validated);
    }
    return compiled.output;
  }
  return "";
}
