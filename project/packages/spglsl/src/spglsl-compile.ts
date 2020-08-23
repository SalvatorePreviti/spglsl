import { GlslInfoLogArray, GlslInfoLogRow } from './glsl-info-log'
import { _wasmSpglslGet } from './lib/_wasm'
import { SpglslLanguage, spglslLanguageFromString, SpglslCompileMode, SpglslPrecision } from './spglsl-enums'
import { StringEnum } from './core/string-enums'
import { SpglslResourceLimits } from './spglsl-resource-limits'

export interface SpglslAngleCompileInput {
  compileMode: SpglslCompileMode
  mainFilePath?: string
  mainSourceCode?: string | null
  language?: string
  customData?: any
  resourceLimits?: Partial<SpglslResourceLimits>
  floatPrecision?: SpglslPrecision
  intPrecision?: SpglslPrecision
}

export class SpglslAngleCompileResult {
  public compileMode: SpglslCompileMode
  public mainFilePath: string
  public language: SpglslLanguage
  public outputVersion: number
  public valid: boolean
  public customData: any | undefined
  public output: string | null
  public infoLog: GlslInfoLogArray
  public floatPrecision: SpglslPrecision
  public intPrecision: SpglslPrecision

  public constructor() {
    this.compileMode = 'Validate'
    this.language = 'Fragment'
    this.mainFilePath = ''
    this.outputVersion = 300
    this.valid = false
    this.customData = undefined
    this.output = null
    this.infoLog = new GlslInfoLogArray()
    this.floatPrecision = ''
    this.intPrecision = ''
  }
}

export async function spglslAngleCompile(input: Readonly<SpglslAngleCompileInput>): Promise<SpglslAngleCompileResult> {
  const result = new SpglslAngleCompileResult()
  if (!StringEnum.has(SpglslCompileMode, input.compileMode)) {
    throw new TypeError(`Invalid compile mode "${input.compileMode}"`)
  }
  result.compileMode = input.compileMode
  result.customData = input.customData
  result.floatPrecision = (StringEnum.has(SpglslPrecision, input.floatPrecision) && input.floatPrecision) || ''
  result.intPrecision = (StringEnum.has(SpglslPrecision, input.intPrecision) && input.intPrecision) || ''
  const resourceLimits = { ...SpglslResourceLimits, ...input.resourceLimits }

  const mainFilePath = input.mainFilePath || '0'
  result.mainFilePath = mainFilePath

  let language: SpglslLanguage | ''
  if (!input.language || input.language === 'auto' || input.language === 'AUTO' || input.language === 'Auto') {
    language = spglslLanguageFromString(mainFilePath) || 'Fragment'
  } else {
    language = spglslLanguageFromString(input.language) || (input.language as SpglslLanguage)
    if (!StringEnum.has(SpglslLanguage, language)) {
      throw new TypeError(`Invalid language "${input.language}"`)
    }
  }
  result.language = language

  const wasm = await _wasmSpglslGet()
  const wresult = wasm.spglsl.spglsl_angle_compile(result, resourceLimits, input.mainSourceCode || '') || {}

  result.infoLog.parseAdd(wresult.infoLog, mainFilePath)

  let valid = !!wresult.valid
  if (valid && result.compileMode !== 'Validate' && (wresult.output === null || wresult.output === undefined)) {
    valid = false
  }

  if (!valid && !result.infoLog.hasErrors()) {
    result.infoLog.push(new GlslInfoLogRow('ERROR', mainFilePath, 0, '', 'compilation errors.'))
    return result
  }

  result.infoLog.removeDuplicates()

  result.valid = !result.infoLog.hasErrors()
  result.output = typeof wresult.output === 'string' ? wresult.output : null

  return result
}
