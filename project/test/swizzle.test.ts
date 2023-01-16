import { expect } from "chai";
import { makeTestShader } from "./lib/test-shader";

describe("swizzle", function () {
  this.timeout(7000);

  const testSwizzle = async (vecSize: number, swizzle: string, expectedSwizzle: string) => {
    const type = `vec${vecSize}`;
    const compiled = await makeTestShader(
      {
        name: swizzle,
        sourceCode: `#version 300 es
      precision highp float;
      uniform ${type} v;
      layout(location=0) out ${type} o;
      void main() {
        o = v.${swizzle};
      }
    `,
      },
      { compileMode: "Optimize", minify: true },
    )();

    const output = compiled.output;
    if (typeof output !== "string") {
      throw new Error("output is not a string");
    }

    let outputSwizzle = "";
    let indexOfStart = output.indexOf("{o=v.");
    if (indexOfStart > 0) {
      indexOfStart += 5;
      const indexOfEnd = output.indexOf(";", indexOfStart);
      outputSwizzle = output.slice(indexOfStart, indexOfEnd);
    }
    expect(outputSwizzle).to.equal(expectedSwizzle);
    return outputSwizzle;
  };

  it(`removes vec2.xy`, () => testSwizzle(2, "xy", ""));
  it(`removes vec3.xyz`, () => testSwizzle(3, "xyz", ""));
  it(`removes vec4.xyzw`, () => testSwizzle(4, "xyzw", ""));

  it("keeps vec2.yx", () => testSwizzle(2, "yx", "yx"));
  it("keeps vec3.yxz", () => testSwizzle(3, "yxz", "yxz"));
  it("keeps vec4.yxzw", () => testSwizzle(4, "yxzw", "yxzw"));

  it("keeps vec3.zyx", () => testSwizzle(3, "zyx", "zyx"));
  it("keeps vec4.wzyx", () => testSwizzle(4, "wzyx", "wzyx"));

  it("keeps vec2.xx", () => testSwizzle(2, "xx", "xx"));
  it("keeps vec3.xxx", () => testSwizzle(3, "xxx", "xxx"));
  it("keeps vec4.xxxx", () => testSwizzle(4, "xxxx", "xxxx"));

  it("keeps vec2.yy", () => testSwizzle(2, "yy", "yy"));
  it("keeps vec3.yyy", () => testSwizzle(3, "yyy", "yyy"));
  it("keeps vec4.yyyy", () => testSwizzle(4, "yyyy", "yyyy"));

  it("keeps vec3.zzz", () => testSwizzle(3, "zzz", "zzz"));
  it("keeps vec4.zzzz", () => testSwizzle(4, "zzzz", "zzzz"));

  it("keeps vec4.wwww", () => testSwizzle(4, "wwww", "wwww"));

  it("optimizes vec2.yx.xy", () => testSwizzle(2, "yx.xy", "yx"));
  it("optimizes vec3.yxz.xyz", () => testSwizzle(3, "yxz.xyz", "yxz"));
  it("optimizes vec4.yxzw.xyzw", () => testSwizzle(4, "yxzw.xyzw", "yxzw"));

  it("optimizes vec2.xx.xy", () => testSwizzle(2, "xx.xy", "xx"));
  it("optimizes vec3.xxx.xyz", () => testSwizzle(3, "xxx.xyz", "xxx"));
  it("optimizes vec4.xxxx.xyzw", () => testSwizzle(4, "xxxx.xyzw", "xxxx"));

  it("optimizes vec2.yx.yx", () => testSwizzle(2, "yx.yx", ""));
  it("optimizes vec3.yxz.yxz", () => testSwizzle(3, "yxz.yxz", ""));
  it("optimizes vec4.yxwz.yxwz", () => testSwizzle(4, "yxwz.yxwz", ""));
});
