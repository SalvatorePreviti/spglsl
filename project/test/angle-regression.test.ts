import { expect } from 'chai'
import { spglslPreload, spglslAngleCompile } from 'spglsl'
import chalk = require('chalk')

import { getTestShaders, TestShader } from './shaders/test-shaders'

describe('angle-regression', function () {
  this.timeout(7000)

  before(async () => {
    await spglslPreload()
  })

  const doTestShader = async (shader: TestShader) => {
    const compiled = await spglslAngleCompile({
      compileMode: 'Optimize',
      mainFilePath: shader.name,
      mainSourceCode: shader.sourceCode,
      language: shader.shaderType
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
      mainSourceCode: compiled.output,
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
  }

  for (const testShader of getTestShaders()) {
    if (testShader.hasIncludes) {
      continue
    }

    it(`ANGLE regression ./${testShader.name}`, async () => {
      Reflect.defineProperty(doTestShader, 'name', {
        value: `./${testShader.name}`,
        configurable: true,
        enumerable: false,
        writable: true
      })
      return doTestShader(testShader)
    })
  }
})
