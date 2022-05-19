import chalk from "chalk";
import { GlslInfoLogArray, GlslInfoLogRow } from "./glsl-info-log";
import { _wasmSpglslGet } from "./lib/_wasm";
import { SpglslLanguage, spglslLanguageFromString, SpglslCompileMode, SpglslPrecision } from "./spglsl-enums";
import { StringEnum } from "./core/string-enums";
import { SpglslResourceLimits } from "./spglsl-resource-limits";
import { makePathRelative, prettySize } from "./core/utils";

const DEFAULT_RECORD_CONSTANT_PRECISION = false;

export interface SpglslAngleCompileOptions {
  compileMode?: SpglslCompileMode;
  language?: string;
  customData?: unknown;
  resourceLimits?: Partial<SpglslResourceLimits>;
  floatPrecision?: SpglslPrecision;
  intPrecision?: SpglslPrecision;
  minify?: boolean;
  mangle?: boolean;
  recordConstantPrecision?: boolean;
}

export interface SpglslAngleCompileInput extends SpglslAngleCompileOptions {
  mainFilePath?: string;
  mainSourceCode: string;
  cwd?: string;
}

export class SpglslAngleCompileResult {
  public compileMode: SpglslCompileMode;
  public mainFilePath: string;
  public language: SpglslLanguage;
  public outputVersion: number;
  public valid: boolean;
  public customData: unknown | undefined;

  /** The time it took to compile, in milliseconds */
  public duration: number;

  /** The source code input */
  public source: string;

  /** The output or null if there is no output */
  public output: string | null;

  public infoLog: GlslInfoLogArray;
  public floatPrecision: SpglslPrecision;
  public intPrecision: SpglslPrecision;
  public minify: boolean;
  public mangle: boolean;
  public recordConstantPrecision: boolean;
  public cwd: string | undefined;

  public constructor() {
    this.source = "";
    this.compileMode = "Validate";
    this.language = "Fragment";
    this.mainFilePath = "";
    this.outputVersion = 300;
    this.valid = false;
    this.customData = undefined;
    this.output = null;
    this.infoLog = new GlslInfoLogArray();
    this.floatPrecision = "";
    this.intPrecision = "";
    this.minify = false;
    this.mangle = false;
    this.recordConstantPrecision = DEFAULT_RECORD_CONSTANT_PRECISION;
    this.duration = 0;
    this.cwd = undefined;
  }
}

export async function spglslAngleCompile(input: Readonly<SpglslAngleCompileInput>): Promise<SpglslAngleCompileResult> {
  const startTime = process.hrtime();

  const result = new SpglslAngleCompileResult();
  result.compileMode = input.compileMode || SpglslCompileMode.Optimize;
  if (!StringEnum.has(SpglslCompileMode, input.compileMode)) {
    throw new TypeError(`Invalid compile mode "${input.compileMode}"`);
  }
  result.customData = input.customData;
  result.floatPrecision = (StringEnum.has(SpglslPrecision, input.floatPrecision) && input.floatPrecision) || "";
  result.intPrecision = (StringEnum.has(SpglslPrecision, input.intPrecision) && input.intPrecision) || "";
  result.minify = !!input.minify;
  result.mangle = input.mangle === undefined ? result.minify : !!input.mangle;
  result.recordConstantPrecision = input.recordConstantPrecision || DEFAULT_RECORD_CONSTANT_PRECISION;
  result.cwd = input.cwd;
  const resourceLimits = { ...SpglslResourceLimits, ...input.resourceLimits };

  const mainFilePath = input.mainFilePath || "0";
  result.mainFilePath = mainFilePath;

  let language: SpglslLanguage | "";
  if (!input.language || input.language === "auto" || input.language === "AUTO" || input.language === "Auto") {
    language = spglslLanguageFromString(mainFilePath) || "Fragment";
  } else {
    language = spglslLanguageFromString(input.language) || (input.language as SpglslLanguage);
    if (!StringEnum.has(SpglslLanguage, language)) {
      throw new TypeError(`Invalid language "${input.language}"`);
    }
  }
  result.language = language;
  if (input.mainSourceCode !== null && input.mainSourceCode !== undefined) {
    result.source = input.mainSourceCode;
  }

  const wasm = await _wasmSpglslGet();
  const wresult = wasm.spglsl.spglsl_angle_compile(result, resourceLimits, input.mainSourceCode || "") || {};

  result.infoLog.parseAdd(wresult.infoLog, mainFilePath, undefined, result.cwd);

  let valid = !!wresult.valid;
  if (valid && result.compileMode !== "Validate" && (wresult.output === null || wresult.output === undefined)) {
    valid = false;
  }

  const timeDiffiff = process.hrtime(startTime);
  result.duration = Math.ceil((timeDiffiff[0] * 1e9 + timeDiffiff[1]) * 1e-6);

  if (!valid && !result.infoLog.hasErrors()) {
    result.infoLog.push(new GlslInfoLogRow("ERROR", mainFilePath, 0, "", "compilation errors.", result.cwd));
    return result;
  }

  result.infoLog.removeDuplicates();

  result.valid = !result.infoLog.hasErrors();
  result.output = typeof wresult.output === "string" ? wresult.output : null;

  return result;
}

export class SpglslAngleCompileError extends Error {
  public code: "SPGLSL_ERR";

  public spglslResult: SpglslAngleCompileResult;

  public filePath: string;

  public durationMs: number;

  public get infoLog(): GlslInfoLogArray {
    return this.spglslResult.infoLog;
  }

  public constructor(spglslResult: SpglslAngleCompileResult, message?: string, caller?: Function) {
    super(message || `spglsls  ${spglslResult.compileMode || "compilation"} failed`);
    this.spglslResult = spglslResult;
    this.code = "SPGLSL_ERR";
    this.filePath = spglslResult.mainFilePath;
    this.durationMs = spglslResult.duration;
    if (spglslResult.infoLog.hasErrors()) {
      Reflect.defineProperty(this, "stack", {
        value: spglslResult.infoLog.inspect(),
        configurable: true,
        enumerable: true,
        writable: true,
      });
    } else if (caller) {
      Error.captureStackTrace(this, caller);
    }
    Reflect.defineProperty(this, "spglslResult", {
      value: spglslResult,
      configurable: true,
      enumerable: false,
      writable: true,
    });
  }
}

export function inspectSpglslAngleCompileResult(result: SpglslAngleCompileResult): string {
  const infologCounts = result.infoLog.getCounts();
  const filePath =
    (result.mainFilePath && makePathRelative(result.mainFilePath, result.cwd)) || result.mainFilePath || "glsl";

  let text: string;
  if (!result.valid) {
    text = chalk.redBright(`${filePath} errors `);
  } else {
    text = chalk.blueBright(`${filePath} `);

    if (infologCounts.warnings) {
      text += chalk.yellow("warnings");
    } else {
      text += chalk.greenBright("ok");
    }

    const sizeOriginal = prettySize(result.source);
    if (sizeOriginal) {
      text += ` ${chalk.cyan(sizeOriginal)}`;

      if (result.compileMode !== "Validate" && typeof result.output === "string") {
        const sizeAfter = prettySize(result.output);
        if (sizeAfter !== sizeOriginal) {
          text += ` ${chalk.blueBright("->")} ${chalk.cyanBright(sizeAfter)}`;
        }
      }
    }
  }

  text += chalk.gray(`  ${result.duration} ms`);

  if (infologCounts.errors || infologCounts.warnings) {
    text += `\n${result.infoLog.inspect()}`;
  }

  return text;
}
