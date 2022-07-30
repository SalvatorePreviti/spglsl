/* eslint-disable no-console */
import path from "path";
import chalk from "chalk";
import type { SpglslAngleCompileOptions, SpglslAngleCompileResult } from "./spglsl-compile";
import { inspectSpglslAngleCompileResult, spglslAngleCompile, SpglslAngleCompileError } from "./spglsl-compile";

export interface RollupPluginSpglslOptions extends SpglslAngleCompileOptions {
  isServer?: boolean;
  isProduction?: boolean;
  extensions?: string[];
  throwOnError?: boolean;
  logging?: boolean;
  cwd?: string;

  onSpglslDone?: (spglslResult: SpglslAngleCompileResult) => void | string | Promise<void> | Promise<string>;
}

export interface RollupPluginSpglslResult extends SpglslAngleCompileResult {
  isServer: boolean;
  isProduction: boolean;
  throwOnError: boolean;
}

/**
 * A plugin for rollup and vite
 */
export function rollupPluginSpglsl(options: RollupPluginSpglslOptions) {
  options = { ...options };

  if (options.minify === undefined && options.compileMode === "Optimize") {
    options.minify = true;
  }

  const extensionsSet = new Set(options.extensions || rollupPluginSpglsl.defaultExtensions);

  function finalizeSettings(isProduction: boolean) {
    options.isProduction = !!isProduction;
    if (!options.cwd) {
      options.cwd = process.cwd();
    }
    if (options.compileMode === undefined) {
      options.compileMode = isProduction ? "Optimize" : "Validate";
    }
    if (options.minify === undefined) {
      options.minify = isProduction && options.compileMode === "Optimize";
    }
  }

  // This hook get called with the final resolved vite config.
  const configResolved = ({ isProduction, command }: { isProduction?: boolean; command?: string }) => {
    if (options.isServer === undefined) {
      options.isServer = command === "serve";
    }
    if (options.throwOnError === undefined) {
      options.throwOnError = !options.isServer;
    }
    if (isProduction !== undefined && options.isProduction === undefined) {
      finalizeSettings(isProduction);
    }
    if (options.logging === undefined) {
      options.logging = true;
    }
  };

  async function spglslTransform(code: string, id: string) {
    if (options.isProduction === undefined) {
      finalizeSettings(process.env.NODE_ENV === "production");
    }

    if (!extensionsSet.has(path.posix.extname(id))) {
      return undefined;
    }

    const spglslResult = (await spglslAngleCompile({
      ...options,
      mainFilePath: id,
      mainSourceCode: code,
    })) as RollupPluginSpglslResult;

    spglslResult.isServer = !!options.isServer;
    spglslResult.isProduction = !!options.isProduction;
    spglslResult.throwOnError = !!options.throwOnError;

    if (!spglslResult.output) {
      spglslResult.output = code;
    }

    if (options.onSpglslDone) {
      await options.onSpglslDone(spglslResult);
    }

    if (!spglslResult.valid) {
      if (spglslResult.throwOnError) {
        throw new SpglslAngleCompileError(
          spglslResult,
          options.isProduction
            ? undefined
            : `spglsl ${spglslResult.compileMode} ${id}:${spglslResult.infoLog.toString({
                colors: false,
              })}`,
        );
      } else if (options.logging) {
        console.error(
          `\n${chalk.blue(`spglsl:${spglslResult.compileMode}`)} ${inspectSpglslAngleCompileResult(spglslResult)}`,
        );
      }
    } else {
      if (options.logging) {
        console.info(
          `\n${chalk.blue(`spglsl:${spglslResult.compileMode}`)} ${inspectSpglslAngleCompileResult(spglslResult)}`,
        );
      }
      if (options.isProduction) {
        const validateResult = await spglslAngleCompile({
          ...options,
          compileMode: "Validate",
          minify: false,
          mangle: false,
          mangle_global_map: undefined,
          mainSourceCode: spglslResult.output,
          mainFilePath: id,
        });
        if (!validateResult.valid) {
          throw new SpglslAngleCompileError(validateResult, "post validation failed");
        }
      }
    }

    if (spglslResult.mangle && spglslResult.mangle_global_map) {
      for (const type of ["uniforms", "globals"] as const) {
        for (const [key, value] of Object.entries(spglslResult[type])) {
          if (key === value && !spglslResult.mangle_global_map[key] && value.length > 2) {
            console.warn(
              chalk.yellow(`WARN: ${type.endsWith("s") ? type.slice(0, type.length - 1) : type} ${key} is not mangled`),
            );
          }
        }
      }
    }

    const r = JSON.stringify(spglslResult.compileMode === "Validate" ? spglslResult.source : spglslResult.output);

    let js: string;

    js = `export const code=${r};\n`;
    js += "export default code;\n";

    js += `\nexport const uniformNames = ${JSON.stringify(spglslResult.uniforms)};\n`;
    for (const [key, value] of Object.entries(spglslResult.uniforms)) {
      js += `export const uniformName_${key}=${JSON.stringify(value)};\n`;
    }

    js += `\nexport const globalNames = ${JSON.stringify(spglslResult.globals)};\n`;
    for (const [key, value] of Object.entries(spglslResult.globals)) {
      js += `export const globalName_${key}=${JSON.stringify(value)};\n`;
    }

    js += `\nexport const constDefs = ${JSON.stringify(spglslResult.constDefs)};\n`;
    for (const [key, value] of Object.entries(spglslResult.constDefs)) {
      js += `export const constDef_${key}=${JSON.stringify(value)};\n`;
    }

    return { code: js, map: null };
  }

  return {
    name: "spglsl",
    configResolved,
    transform: spglslTransform,
  };
}

rollupPluginSpglsl.defaultExtensions = [".frag", ".vert", ".comp"];
