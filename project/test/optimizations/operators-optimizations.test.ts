import { expect } from "chai";
import { spglslAngleCompile, SpglslAngleCompileError } from "spglsl";

const SHADER_PREFIX =
  "#version 300 es\nprecision mediump float;uniform int iN;uniform bool iB;uniform vec4 vA,vB;layout(location=1)out vec4 V;layout(location=2)out vec4 P;layout(location=3)out int N;";

describe("operators-optimizations", function () {
  this.timeout(7000);

  it("removes positive operator", async () => {
    expect(await compileMain("P.x=+P.y;")).to.eq("P.x=P.y;");
    expect(await compileMain("P.x=+ + +P.y;")).to.eq("P.x=P.y;");
  });

  it("removes double negations", async () => {
    expect(await compileMain("P.x=-P.y;")).to.eq("P.x=-P.y;");
    expect(await compileMain("P.x=- -P.y;")).to.eq("P.x=P.y;");
    expect(await compileMain("P.x=- - -P.y;")).to.eq("P.x=-P.y;");
    expect(await compileMain("P.x=- - - -P.y;")).to.eq("P.x=P.y;");
  });

  it("removes double bitwise not", async () => {
    expect(await compileMain("N=~iN;")).to.eq("N=~iN;");
    expect(await compileMain("N=~~iN;")).to.eq("N=iN;");
    expect(await compileMain("N=~~~iN;")).to.eq("N=~iN;");
    expect(await compileMain("N=~~~~iN;")).to.eq("N=iN;");
  });

  it("removes double logical not", async () => {
    expect(await compileMain("N=int(!iB);")).to.eq("N=int(!iB);");
    expect(await compileMain("N=int(!!iB);")).to.eq("N=int(iB);");
    expect(await compileMain("N=int(!!!iB);")).to.eq("N=int(!iB);");
    expect(await compileMain("N=int(!!!!iB);")).to.eq("N=int(iB);");
  });

  it("Optimizes boolean negations", async () => {
    expect(await compileMain("N=int(!(vA==vB));")).to.eq("N=int(vA!=vB);");
    expect(await compileMain("N=int(!(vA!=vB));")).to.eq("N=int(vA==vB);");
    expect(await compileMain("N=int(!(vA.x>vB.x));")).to.eq("N=int(vA.x<=vB.x);");
    expect(await compileMain("N=int(!(vA.x<vB.x));")).to.eq("N=int(vA.x>=vB.x);");
    expect(await compileMain("N=int(!(vA.x>=vB.x));")).to.eq("N=int(vA.x<vB.x);");
    expect(await compileMain("N=int(!(vA.x<=vB.x));")).to.eq("N=int(vA.x>vB.x);");
  });

  it("Optimizes ternary negation", async () => {
    expect(await compileMain("P.x=!(vA.x>vB.x)?vA.z:vA.w;")).to.eq("P.x=vA.x>vB.x?vA.w:vA.z;");
  });
});

async function compileMain(code: string): Promise<string> {
  let result = await compile(`void main(){${code}}`);
  result = result.replace("void main(){", "");
  if (result.endsWith("}")) {
    result = result.slice(0, -1);
  }
  return result;
}

async function compile(code: string): Promise<string> {
  const compiled = await spglslAngleCompile({
    mainSourceCode: SHADER_PREFIX + code,
    compileMode: "Optimize",
    mangle: false,
    minify: false,
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
    return compiled.output.replace(SHADER_PREFIX, "");
  }
  return "";
}
