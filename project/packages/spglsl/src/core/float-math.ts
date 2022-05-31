const { isFinite: numberIsFinite, isNaN: numberIsNan, parseFloat: numberParseFloat } = Number;
const { min, max, abs, fround, round } = Math;
import floatConsts from "./float-consts.min.json";
const floatConstsReverse = Object.fromEntries(Object.entries(floatConsts).map(([a, b]) => [b, a])) as Record<
  string,
  string
>;

export const FLOAT_MIN = 1.17549435082228750797e-38;

export const FLOAT_MAX = 3.40282346638528859812e38;

export const FLOAT_EPSILON = 1.1920928955078125e-7;

const _zeroRegex = /^0*|\+/g;
const _expTrailingZeroesRegex = /(?:(\.[1-9]+)|\.)0*e/gi;
const _trailingZeroesRegex = /0+$/;

const _floatToStrCache = new Map<number, string>();

export function glslToInt32(value: string | number | boolean | null | undefined): number {
  return glslToFloat(value) | 0;
}

export function glslToUInt32(value: string | number | boolean | null | undefined): number {
  return glslToFloat(value) >>> 0;
}

export function glslToFloat(value: string | number | boolean | null | undefined): number {
  if (typeof value === "string") {
    const found = floatConstsReverse[value];
    if (typeof found === "string") {
      value = found;
    }
  }

  if (typeof value === "number") {
    if (!numberIsFinite(value)) {
      return value;
    }
    value = fround(max(-FLOAT_MAX, min(FLOAT_MAX, value)));
    return abs(value) >= FLOAT_MIN ? value : 0;
  }

  if (value === "Infinity" || value === "+Infinity") {
    return Infinity;
  }
  if (value === "-Infinity") {
    return -Infinity;
  }
  if (value === "NaN") {
    return NaN;
  }
  if (value === true || value === "true" || value === "!0") {
    return 1;
  }
  if (value === null || value === undefined || value === false || value === "false" || value.startsWith("!")) {
    return 0;
  }

  if (value.charCodeAt(0) === 40 && value.charCodeAt(value.length - 1) === 41) {
    value = value.slice(1, value.length - 1);
  }

  const indexOfSlash = value.indexOf("/");
  if (indexOfSlash > 0) {
    const splitted = value.split("/");
    const numeratorStr = splitted[0];
    const denominatorStr = splitted[1];
    if (numeratorStr && denominatorStr) {
      return glslToFloat(glslToFloat(numeratorStr) / glslToFloat(denominatorStr));
    }
  }
  if (value.indexOf("0x") >= 0) {
    return parseInt(value);
  }
  return glslToFloat(parseFloat(value));
}

export function int32ToGlsl(value: string | number | boolean | null | undefined): string {
  const n = glslToInt32(value);
  const decStr = n.toString(10);
  const hexStr = `${n < 0 ? "-" : ""}0x${Math.abs(n).toString(16)}`;
  return decStr.length <= hexStr.length ? decStr : hexStr;
}

export function uint32ToGlsl(value: string | number | boolean | null | undefined): string {
  const n = glslToUInt32(value);
  const decStr = `${n.toString(10)}u`;
  const hexStr = `0x${n.toString(16)}u`;
  return decStr.length <= hexStr.length ? decStr : hexStr;
}

export function floatToGlsl(
  value: string | number | boolean | null | undefined,
  needsParentheses: boolean = false,
  needsFloat: boolean = true,
) {
  const r = floatToGlslRaw(value, needsParentheses, needsFloat);
  const found = (floatConsts as Record<string, string>)[r];
  return typeof found === "string" ? found : r;
}

export function floatToGlslRaw(
  value: string | number | boolean | null | undefined,
  needsParentheses: boolean = false,
  needsFloat: boolean = true,
): string {
  if (typeof value !== "number") {
    value = glslToFloat(value);
  }

  if (!numberIsFinite(value)) {
    const special = numberIsNan(value) ? "0./0." : value < 0 ? "-1./0." : "1./0.";
    return needsParentheses ? `(${special})` : special;
  }
  value = fround(max(-FLOAT_MAX, min(FLOAT_MAX, value)));
  const absValue = value < 0 ? fround(-value) : value;
  if (!absValue || absValue < FLOAT_MIN) {
    return needsFloat ? "0." : "0";
  }

  const rounded = fround(round(value));
  if (rounded === value) {
    const s = rounded.toString();
    if (s.indexOf(".") < 0 && s.indexOf("e") < 0) {
      return needsFloat ? `${s}.` : s;
    }
  }

  let decimal = _floatToStrCache.get(absValue);
  if (decimal === undefined) {
    decimal = _floatToStr(absValue);
    if (_floatToStrCache.size > 10000) {
      _gcCache(_floatToStrCache, 8000);
    }
    _floatToStrCache.set(absValue, decimal);
  }

  let best: string;
  best = value < 0 ? `-${decimal}` : decimal;
  if (!needsFloat && best.endsWith(".")) {
    best = best.slice(0, best.length - 1);
  }

  return best;
}

function _gcCache(map: Map<unknown, unknown>, maxSize: number) {
  for (const k of map.keys()) {
    if (map.size <= maxSize || !map.delete(k)) {
      break;
    }
  }
}

function _floatToStr(absValue: number): string {
  absValue = Math.fround(absValue);
  let best = absValue.toLocaleString("en", {
    useGrouping: false,
    maximumFractionDigits: 8,
  });

  const tostr = absValue.toString();
  if (!best || tostr.length < best.length) {
    best = tostr;
  }

  let digits = 9;
  if (best.indexOf("e") < 0) {
    const indexOfDot = best.indexOf(".");
    if (indexOfDot >= 0) {
      digits = best.length - indexOfDot - 1;
    }
    best = best.replace(_zeroRegex, "").replace(_expTrailingZeroesRegex, "$1e");
  }

  if (!best) {
    return "0.";
  }

  let prevLen = 0;
  do {
    let s = absValue.toFixed(digits);
    if (s.indexOf("e") >= 0) {
      break;
    }
    const indexOfDot = s.indexOf(".");
    if (indexOfDot < 0) {
      digits = 0;
    } else {
      s = s.replace(_trailingZeroesRegex, "");
      digits = s.length - indexOfDot - 2;
    }
    if (fround(numberParseFloat(s)) !== absValue) {
      break;
    }
    best = s;
    if (prevLen === s.length) {
      break;
    }
    prevLen = s.length;
  } while (digits > 0);
  best = best.replace(_zeroRegex, "");

  if (!best) {
    return "0.";
  }

  let bestExp = absValue.toExponential();
  digits = 9;
  do {
    const s = absValue.toExponential(digits--);
    if (fround(numberParseFloat(s)) !== absValue) {
      break;
    }
    bestExp = s;
  } while (digits > 0);
  bestExp = bestExp.replace(_zeroRegex, "").replace(_expTrailingZeroesRegex, "$1e");
  return (bestExp.length < best.length ? bestExp : best) || "0";
}
