import type { StringEnumValue } from "./core/string-enums";
import { StringEnum } from "./core/string-enums";
import { extname as pathExtname, basename as pathBasename } from "path";

export const SpglslPrecision = StringEnum("", "lowp", "mediump", "highp");

// eslint-disable-next-line @typescript-eslint/no-redeclare
export type SpglslPrecision = StringEnumValue<typeof SpglslPrecision>;

export const SpglslCompileMode = StringEnum("Validate", "Compile", "Optimize");

// eslint-disable-next-line @typescript-eslint/no-redeclare
export type SpglslCompileMode = StringEnumValue<typeof SpglslCompileMode>;

export const SpglslLanguage = StringEnum(
  "Vertex",
  "TessControl",
  "TessEvaluation",
  "Geometry",
  "Fragment",
  "Compute",
  "RayGen",
  "Intersect",
  "AnyHit",
  "ClosestHit",
  "Miss",
  "Callable",
);

// eslint-disable-next-line @typescript-eslint/no-redeclare
export type SpglslLanguage = StringEnumValue<typeof SpglslLanguage>;

export function spglslLanguageFromString(s: string | null | undefined): SpglslLanguage | "" {
  if (!s) {
    return "";
  }

  if (StringEnum.has(SpglslLanguage, s)) {
    return s;
  }

  let basename: string | null = null;
  if (s.indexOf("/") >= 0 || s.indexOf("\\") >= 0) {
    s = pathExtname(s) || s;
    if (s && s.startsWith(".")) {
      basename = pathBasename(s, s);
      s = s.slice(1);
    }
    s = (s && s.toLowerCase()) || "";
  }

  if (s.startsWith("gl_")) {
    s = s.slice(3);
  }

  if (s.startsWith("frag") || s.startsWith("fs")) {
    return "Fragment";
  }

  if (s.startsWith("vert") || s.startsWith("vs")) {
    return "Vertex";
  }

  if (s.startsWith("geom") || s.startsWith("gs")) {
    return "Geometry";
  }

  if (s.startsWith("comp") || s.startsWith("cs")) {
    return "Compute";
  }

  if (s.startsWith("tesc") || s.startsWith("tsc")) {
    return "TessControl";
  }

  if (s.startsWith("tes") || s.startsWith("tse")) {
    return "TessEvaluation";
  }

  if (basename) {
    const indexOfSep = Math.max(basename.lastIndexOf("."), basename.lastIndexOf("-"), basename.lastIndexOf("_"));
    if (indexOfSep > 0) {
      return spglslLanguageFromString(basename.slice(indexOfSep + 1));
    }
  }

  return "";
}

export function spglslParseVersion(
  version: string | number | null | undefined,
  language: SpglslLanguage = "Fragment",
): number {
  switch (version) {
    case "Fragment":
    case "Vertex":
    case "GLES2":
    case "WEBGL":
    case "GLES3":
    case "WEBGL2":
      return 300;

    case "GLES3_1":
    case "WEBGL3":
      return 310;

    default:
      return typeof version === "number" ? version | 0 : language === "Fragment" || language === "Vertex" ? 300 : 310;
  }
}
