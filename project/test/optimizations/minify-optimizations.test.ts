import { expect } from "chai";
import { spglslAngleCompile, SpglslAngleCompileError } from "spglsl";

const SHADER_PREFIX =
  "#version 300 es\nprecision mediump float;layout(location=1)out vec4 V;layout(location=2)out vec4 P;";

describe("minify-optimizations", function () {
  this.timeout(7000);

  describe("minifications", async () => {
    it("convert statements into commas", async () => {
      expect(await compileMain("{{P.x=2.;}{{V.y=1.;}}}")).to.eq("P.x=2.,V.y=1.;");
    });

    it("replaces if else with ternary (when possible)", async () => {
      expect(await compileMain("P.z=3.;if (P.x==1.) {P.x=2.;} else P.y=2.;")).to.eq("P.z=3.,P.x==1.?P.x=2.:P.y=2.;");

      expect(
        await compile(
          "bool boolFunc(){P.x=1.;return P.y>1.;} void main() { P.z=3.;boolFunc();if (P.x==1.) {P.x=2.;} else P.y=2.; }",
        ),
      ).to.eq("bool boolFunc(){return P.x=1.,P.y>1.;}void main(){P.z=3.,boolFunc(),P.x==1.?P.x=2.:P.y=2.;}");
    });

    it("replaces if with && (when possible)", async () => {
      expect(
        await compile("bool boolFunc(){P.x=1.;return P.y>1.;} void main() { if (P.x==1.) {{boolFunc();}} }"),
      ).to.eq("bool boolFunc(){return P.x=1.,P.y>1.;}void main(){P.x==1.&&boolFunc();}");
    });

    it("optimizes ternary assignments with or without commas", async () => {
      expect(await compileMain("if(V.x>0.){P.x=1.;P.y=2.;}else{P.x=2.;P.y=7.;}")).to.eq(
        "P.y=V.x>0.?P.x=1.,2.:(P.x=2.,7.);",
      );

      expect(await compileMain("if(V.x>0.){P.y=2.;}else{P.x=2.;P.y=7.;}")).to.eq("P.y=V.x>0.?2.:(P.x=2.,7.);");

      expect(await compileMain("if(V.x>0.){P.x=1.;P.y=2.;}else{P.y=7.;}")).to.eq("P.y=V.x>0.?P.x=1.,2.:7.;");

      expect(await compileMain("if(V.x>0.){P.y=2.;}else{P.y=7.;}")).to.eq("P.y=V.x>0.?2.:7.;");
    });
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
    return compiled.output.replace(SHADER_PREFIX, "");
  }
  return "";
}
