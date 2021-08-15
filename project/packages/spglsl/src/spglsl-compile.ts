import chalk from 'chalk'
import { GlslInfoLogArray, GlslInfoLogRow } from './glsl-info-log'
import { _wasmSpglslGet } from './lib/_wasm'
import { SpglslLanguage, spglslLanguageFromString, SpglslCompileMode, SpglslPrecision } from './spglsl-enums'
import { StringEnum } from './core/string-enums'
import { SpglslResourceLimits } from './spglsl-resource-limits'

const DEFAULT_RECORD_CONSTANT_PRECISION = false

export interface SpglslAngleCompileOptions {
  compileMode?: SpglslCompileMode
  language?: string
  customData?: any
  resourceLimits?: Partial<SpglslResourceLimits>
  floatPrecision?: SpglslPrecision
  intPrecision?: SpglslPrecision
  minify?: boolean
  mangleTwoPasses?: boolean
  recordConstantPrecision?: boolean
}

export interface SpglslAngleCompileInput extends SpglslAngleCompileOptions {
  mainFilePath?: string
  mainSourceCode: string
}

export class SpglslAngleCompileResult {
  public compileMode: SpglslCompileMode
  public mainFilePath: string
  public language: SpglslLanguage
  public outputVersion: number
  public valid: boolean
  public customData: any | undefined
  public mainSourceCode: string

  /** The time it took to compile, in milliseconds */
  public duration: number

  /** The output or null if there is no output */
  public output: string | null

  public infoLog: GlslInfoLogArray
  public floatPrecision: SpglslPrecision
  public intPrecision: SpglslPrecision
  public minify: boolean
  public mangleTwoPasses: boolean
  public recordConstantPrecision: boolean

  public constructor() {
    this.mainSourceCode = ''
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
    this.minify = false
    this.mangleTwoPasses = false
    this.recordConstantPrecision = DEFAULT_RECORD_CONSTANT_PRECISION
    this.duration = 0
  }
}

export function inspectSpglslAngleCompileResult(result: SpglslAngleCompileResult): string {
  let text: string
  if (result.valid) {
    text = chalk.redBright(`- ${result.mainFilePath} not ok.`)
  } else {
    text = chalk.blueBright(`- ${result.mainFilePath} `)
    if (result.mainSourceCode) {
      const sizeOriginal = prettyGlslFileSize(result.mainSourceCode)
      text += chalk.cyan(sizeOriginal)

      const sizeAfter = prettyGlslFileSize(result.output || result.mainSourceCode)
      if (sizeAfter !== sizeOriginal) {
        text += ` -> ${chalk.cyanBright(sizeOriginal)}`
      }
    }
  }

  text += chalk.gray(` ${result.duration} ms`)

  const infologText = result.infoLog.inspect()
  if (infologText) {
    text += `\n${infologText}`
  }

  return text
}

export async function spglslAngleCompile(input: Readonly<SpglslAngleCompileInput>): Promise<SpglslAngleCompileResult> {
  const startTime = process.hrtime()

  const result = new SpglslAngleCompileResult()
  result.compileMode = input.compileMode || SpglslCompileMode.Optimize
  if (!StringEnum.has(SpglslCompileMode, input.compileMode)) {
    throw new TypeError(`Invalid compile mode "${input.compileMode}"`)
  }
  result.customData = input.customData
  result.floatPrecision = (StringEnum.has(SpglslPrecision, input.floatPrecision) && input.floatPrecision) || ''
  result.intPrecision = (StringEnum.has(SpglslPrecision, input.intPrecision) && input.intPrecision) || ''
  result.minify = !!input.minify
  result.mangleTwoPasses = input.mangleTwoPasses === undefined ? true : !!input.mangleTwoPasses
  result.recordConstantPrecision = input.recordConstantPrecision || DEFAULT_RECORD_CONSTANT_PRECISION
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
  if (input.mainSourceCode !== null && input.mainSourceCode !== undefined) {
    result.mainSourceCode = input.mainSourceCode
  }

  const wasm = await _wasmSpglslGet()
  const wresult = wasm.spglsl.spglsl_angle_compile(result, resourceLimits, input.mainSourceCode || '') || {}

  result.infoLog.parseAdd(wresult.infoLog, mainFilePath)

  let valid = !!wresult.valid
  if (valid && result.compileMode !== 'Validate' && (wresult.output === null || wresult.output === undefined)) {
    valid = false
  }

  const timeDiffiff = process.hrtime(startTime)
  result.duration = Math.ceil((timeDiffiff[0] * 1e9 + timeDiffiff[1]) * 1e-6)

  if (!valid && !result.infoLog.hasErrors()) {
    result.infoLog.push(new GlslInfoLogRow('ERROR', mainFilePath, 0, '', 'compilation errors.'))
    return result
  }

  result.infoLog.removeDuplicates()

  result.valid = !result.infoLog.hasErrors()
  result.output = typeof wresult.output === 'string' ? wresult.output : null

  return result
}

export class SpglslAngleCompileError extends Error {
  public code: 'SPGLSL_ERR'

  public spglslResult: SpglslAngleCompileResult

  public filePath: string

  public durationMs: number

  public get infoLog(): GlslInfoLogArray {
    return this.spglslResult.infoLog
  }

  public constructor(spglslResult: SpglslAngleCompileResult, message?: string, caller?: Function) {
    super(message || `spglsls  ${spglslResult.compileMode || 'compilation'} failed`)
    this.spglslResult = spglslResult
    this.code = 'SPGLSL_ERR'
    this.filePath = spglslResult.mainFilePath
    this.durationMs = spglslResult.duration
    if (spglslResult.infoLog.hasErrors()) {
      Reflect.defineProperty(this, 'stack', {
        value: spglslResult.infoLog.inspect(),
        configurable: true,
        enumerable: true,
        writable: true
      })
    } else if (caller) {
      Error.captureStackTrace(this, caller)
    }
    Reflect.defineProperty(this, 'spglslResult', {
      value: spglslResult,
      configurable: true,
      enumerable: false,
      writable: true
    })
  }
}

function humanReadableSizeInBytes(bytes: number): string {
  const i = bytes && (Math.min(Math.floor(Math.log(Math.abs(bytes) / Math.log(1024))), 6) || 0)
  return `${+(bytes / 1024 ** i).toFixed(2)} ${i ? ' kMGTPE'[i] : ''}B`
}

function prettyGlslFileSize(glsl: string) {
  const n = Buffer.byteLength(glsl, 'utf8')
  const s = humanReadableSizeInBytes(n)
  return s + (n >= 1024 && n < 1e12 ? `, ${n} bytes` : s.endsWith(' B') ? 'ytes' : '')
}
