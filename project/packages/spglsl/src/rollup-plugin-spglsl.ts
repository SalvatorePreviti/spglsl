import path from 'path'
import {
  spglslAngleCompile,
  SpglslAngleCompileError,
  SpglslAngleCompileOptions,
  SpglslAngleCompileResult
} from './spglsl-compile'

export interface RollupPluginSpglslOptions extends SpglslAngleCompileOptions {
  isProduction?: boolean
  extensions?: string[]

  onSpglslDone?: (spglslResult: SpglslAngleCompileResult) => void | string | Promise<void> | Promise<string>
}

/**
 * A plugin for rollup and vite
 */
export function rollupPluginSpglsl(options: RollupPluginSpglslOptions) {
  const finalOptions = { ...options }
  if (finalOptions.minify === undefined && options.compileMode === 'Optimize') {
    finalOptions.minify = true
  }

  const extensionsSet = new Set(finalOptions.extensions || rollupPluginSpglsl.defaultExtensions)

  function finalizeSettings(isProduction: boolean) {
    finalOptions.isProduction = !!isProduction
    if (finalOptions.compileMode === undefined) {
      finalOptions.compileMode = isProduction ? 'Optimize' : 'Validate'
    }
    if (finalOptions.minify === undefined) {
      finalOptions.minify = isProduction && finalOptions.compileMode === 'Optimize'
    }
  }

  // This hook get called with the final resolved vite config.
  const configResolved = ({ isProduction }: { isProduction?: boolean }) => {
    if (isProduction !== undefined && finalOptions.isProduction === undefined) {
      finalizeSettings(isProduction)
    }
  }

  async function spglslTransform(this: any, code: string, id: string) {
    if (finalOptions.isProduction === undefined) {
      finalizeSettings(process.env.NODE_ENV === 'production')
    }

    if (!extensionsSet.has(path.posix.extname(id))) {
      return undefined
    }

    const spglslResult = await spglslAngleCompile({
      ...finalOptions,
      mainFilePath: id,
      mainSourceCode: code
    })

    if (!spglslResult.valid) {
      throw new SpglslAngleCompileError(spglslResult)
    }

    code = spglslResult.output || code
    if (finalOptions.onSpglslDone) {
      code = (await finalOptions.onSpglslDone(spglslResult)) || code
    }

    return `export default ${JSON.stringify(code)}`
  }

  return {
    name: 'spglsl',
    configResolved,
    transform: spglslTransform
  }
}

rollupPluginSpglsl.defaultExtensions = ['.frag', '.vert', '.comp']
