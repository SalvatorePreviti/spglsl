import { floatToGlslRaw } from "../packages/spglsl/src/core/float-math";
import fs from "fs";
import path from "path";

const reverseLookupMap: Record<string, string> = {};

function reverseLookupMapAdd(left: number, right: () => string) {
  if (isFinite(left) && Math.abs(left) < 1000000000) {
    left = Math.fround(left);
    const sleft = floatToGlslRaw(left, false, true);
    const sright = right();
    if (sright.length <= sleft.length) {
      const found = reverseLookupMap[sleft];
      if (!found || found.length > sright.length) {
        reverseLookupMap[sleft] = sright;
      }
    }
  }
}

for (let i = -1000; i <= 1000; ++i) {
  const value = Math.fround(i / 1000);
  reverseLookupMapAdd(Math.acos(value), () => `acos(${floatToGlslRaw(value)})`);
}

for (let i = -1000; i <= 1000; ++i) {
  const value = Math.fround(i / 100);
  reverseLookupMapAdd(Math.cos(value), () => `cos(${floatToGlslRaw(value)})`);
}

for (let i = -2000; i <= 2000; ++i) {
  const value = Math.fround(i / 2000);
  reverseLookupMapAdd(Math.asin(value), () => `asin(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.atanh(value), () => `atanh(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.exp(value), () => `exp(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.log(value), () => `log(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.cosh(value), () => `cosh(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.sinh(value), () => `sinh(${floatToGlslRaw(value)})`);
}

for (let i = -1000; i <= 1000; ++i) {
  const value = Math.fround(i / 100);
  reverseLookupMapAdd(Math.sin(value), () => `sin(${floatToGlslRaw(value)})`);
}

for (let i = -20000; i <= 20000; ++i) {
  const value = Math.fround(i / 1000);
  reverseLookupMapAdd(Math.sqrt(value), () => `sqrt(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.exp(value), () => `exp(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(2 ** value, () => `exp2(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.log(value), () => `log(${floatToGlslRaw(value)})`);
}

for (let i = -20000; i <= 20000; ++i) {
  const value = Math.fround(i / 1000);
  reverseLookupMapAdd(Math.atan(value), () => `atan(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.asinh(value), () => `asinh(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.acosh(value), () => `acosh(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.tan(value), () => `tan(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.cosh(value), () => `cosh(${floatToGlslRaw(value)})`);
  reverseLookupMapAdd(Math.sinh(value), () => `sinh(${floatToGlslRaw(value)})`);
}

fs.writeFileSync(
  path.resolve(__dirname, "../packages/spglsl/src/core/float-consts.min.json"),
  JSON.stringify(reverseLookupMap),
);
