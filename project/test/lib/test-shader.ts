import { expect } from 'chai'
import { spglslAngleCompile, SpglslAngleCompileOptions } from 'spglsl'
import chalk from 'chalk'

export interface TestShader {
  name: string
  sourceCode: string
  shaderType?: 'vert' | 'frag'
  hasIncludes?: boolean
}

export const makeTestShader = (shader: TestShader, options?: SpglslAngleCompileOptions) => {
  const testShader = async () => {
    const compiled = await spglslAngleCompile({
      compileMode: 'Optimize',
      language: shader.shaderType,
      minify: true,
      ...options,
      mainFilePath: shader.name,
      mainSourceCode: shader.sourceCode
    })

    const info = `${compiled.compileMode} ${compiled.language} ${compiled.outputVersion} ./${
      shader.name
    }\n${compiled.infoLog.toString({
      colors: true
    })}`

    expect(compiled.valid).to.equal(true, info)
    expect(compiled.infoLog.getCounts().errors).to.equal(0, info)

    console.log(chalk.magentaBright(`----- ${shader.name} -----`))
    console.log(shader.sourceCode)
    console.log(chalk.magentaBright('---------'))
    console.log(chalk.greenBright(compiled.output !== null ? compiled.output : null))
    console.log(chalk.magentaBright('---------'))

    const validated = await spglslAngleCompile({
      compileMode: 'Validate',
      mainFilePath: compiled.mainFilePath,
      mainSourceCode: compiled.output || compiled.mainSourceCode,
      customData: compiled.customData,
      language: compiled.language
    })

    const info1 = `${validated.compileMode} ${validated.language} ${validated.outputVersion} ./${
      shader.name
    }\n${validated.infoLog.toString({
      colors: true
    })} ${compiled.output}`

    expect(validated.valid).to.equal(true, info1)
    expect(validated.infoLog.getCounts().errors).to.equal(0, info)

    console.log(chalk.cyanBright(`${(compiled.output || '').length} bytes`))

    return compiled
  }

  Reflect.defineProperty(testShader, 'name', {
    value: `./${testShader.name}`,
    configurable: true,
    enumerable: false,
    writable: true
  })

  return testShader
}
