import { expect } from "chai";
import { spglslAngleCompile, SpglslAngleCompileError } from "spglsl";

const SHADER_PREFIX =
  "#version 300 es\nprecision mediump float;layout(location=1)out vec4 V;layout(location=2)out vec4 P;";

describe("blocks-optimizations", function () {
  this.timeout(7000);

  describe("blocks optimizations", () => {
    it("removes extra blocks", async () => {
      expect(await compileMain("{{P.x=2.;}{{V.y=1.;}}}")).to.eq("P.x=2.;V.y=1.;");
    });
    it("removes comma operators", async () => {
      expect(await compileMain("P.x=2.,V.y=1.;")).to.eq("P.x=2.;V.y=1.;");
    });
    it("replaces ternary with if else", async () => {
      expect(await compileMain("P.x==1.?P.x=2.:P.y=2.;")).to.eq("if(P.x==1.)P.x=2.;else P.y=2.;");
    });
    it("replace if then else assignment to the same variable with ternary", async () => {
      expect(await compileMain("if (P.x==1.123) { P.y=1.; } else { P.y=3.; }")).to.eq("P.y=P.x==1.123?1.:3.;");
      expect(await compileMain("if (P.x>1.123) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=P.x>1.123?1.:3.;");
    });
    it("remove constant ifs", async () => {
      expect(await compileMain("if (true) { P.y+=1.; }")).to.eq("P.y+=1.;");
      expect(await compileMain("if (true) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=1.;");
      expect(await compileMain("if (false) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=3.;");
      expect(await compileMain("if (false && P.x==0.) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=3.;");
      expect(await compileMain("if (P.z==10. && false && P.x==0.) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=3.;");
      expect(await compileMain("if (P.x==0. || true) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=1.;");
      expect(await compileMain("if (!(P.x==0. || true)) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=3.;");
      expect(await compileMain("if (true ^^ false) { P.y+=1.; } else { P.y+=3.; }")).to.eq("P.y+=1.;");
    });
  });
  describe("loop optimizations", () => {
    it("converts while to for", async () => {
      expect(await compileMain("while (P.x>0.){P.x+=1.;P.y+=2.;}")).to.eq("for(;P.x>0.;){P.x+=1.;P.y+=2.;}");
    });
    it("removes while (false)", async () => {
      expect(await compileMain("while (false){P.x+=1.;P.y+=2.;}")).to.eq("");
    });
    it("removes for (;false;)", async () => {
      expect(await compileMain("for (;false;++P.x){P.x+=1.;P.y+=2.;}")).to.eq("");
    });
    it("makes infinite for loops", async () => {
      expect(await compileMain("for (P.x=1.;true;++P.x){P.x+=1.;}")).to.eq("for(P.x=1.;;++P.x)P.x+=1.;");
      expect(await compileMain("for (P.x=1.;true;P.x){P.x+=1.;}")).to.eq("for(P.x=1.;;)P.x+=1.;");
    });
    it("keeps only init for (P.x=1.;false;)", async () => {
      expect(await compileMain("for (P.x=1.;false;++P.x){P.x+=1.;P.y+=2.;}")).to.eq("P.x=1.;");
    });
    it("remove everything for for (float m=1.;false;)", async () => {
      expect(await compileMain("for (float m=1.;false;++P.x){P.x+=1.;P.y+=2.;}")).to.eq("");
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
