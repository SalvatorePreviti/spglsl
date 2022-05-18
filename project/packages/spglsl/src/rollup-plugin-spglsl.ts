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

  export?: "default" | "named" | "default-and-named";

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
        throw new SpglslAngleCompileError(spglslResult);
      } else if (options.logging) {
        console.error(
          `\n${chalk.blue(`spglsl:${spglslResult.compileMode}`)} ${inspectSpglslAngleCompileResult(spglslResult)}`,
        );
      }
    } else if (options.logging) {
      console.info(
        `\n${chalk.blue(`spglsl:${spglslResult.compileMode}`)} ${inspectSpglslAngleCompileResult(spglslResult)}`,
      );
    }

    const r = JSON.stringify(spglslResult.compileMode === "Validate" ? spglslResult.source : spglslResult.output);

    if (!options.export || options.export === "default") {
      return `export default ${r}`;
    }

    let js = `export let code=${r};\nexport let setCode=(v)=>{code=v;};\n`;
    if (options.export === "default-and-named") {
      js += "export default code;\n";
    }
    return js;
  }

  return {
    name: "spglsl",
    configResolved,
    transform: spglslTransform,
  };
}

rollupPluginSpglsl.defaultExtensions = [".frag", ".vert", ".comp"];
