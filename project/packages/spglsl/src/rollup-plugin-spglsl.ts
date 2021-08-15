import path from 'path'
import {
  spglslAngleCompile,
  SpglslAngleCompileError,
  SpglslAngleCompileOptions,
  SpglslAngleCompileResult
} from './spglsl-compile'

export interface RollupPluginSpglslOptions extends SpglslAngleCompileOptions {
  extensions?: string[]

  onSpglslDone?: (spglslResult: SpglslAngleCompileResult) => void | string | Promise<void> | Promise<string>
}

const defaultOptions: RollupPluginSpglslOptions = {
  extensions: ['.frag', '.vert', '.comp']
}

/**
 * A plugin for vite
 */
export function rollupPluginSpglsl(options: RollupPluginSpglslOptions) {
  const finalOptions = { ...defaultOptions }
  if (finalOptions.minify === undefined && options.compileMode === 'Optimize') {
    finalOptions.minify = true
  }

  const extensionsSet = new Set(finalOptions.extensions)

  async function spglslTransform(this: any, code: string, id: string) {
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
    name: 'vite-plugin-spglsl',
    transform: spglslTransform
  }
}
