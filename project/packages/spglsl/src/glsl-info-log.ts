import _util = require('util')
import _path = require('path')
import _chalk = require('chalk')
import { StringEnum, StringEnumValue } from './core/string-enums'

export const GlslLogRowType = StringEnum('WARNING', 'ERROR', 'UNKNOWN ERROR')

export type GlslLogRowType = StringEnumValue<typeof GlslLogRowType>

const { relative: pathRelative } = _path.posix

let _chalkDisabled: _chalk.Chalk | undefined

const _glslInfoLineRegex = /^(WARNING|ERROR|UNKNOWN ERROR|TBD functionality|Missing functionality|warning|error|Warning|Warn|warn|Error): (?:(-?(?:[a-zA-Z]:)?[^:]+)?:?(-?\d+))?(?:: +)?(?:'(.*)' : )?(.*)$/s

export interface GlslInfoLogFormatOptions {
  colors?: boolean
}

export class GlslInfoLogRow implements GlslInfoLogRow {
  public type: GlslLogRowType
  public filePath: string
  public line: number
  public source: string
  public message: string

  public constructor(type?: GlslLogRowType, filePath?: string, line?: number, source?: string, message?: string) {
    this.type = type || 'ERROR'
    this.filePath = filePath || '0'
    this.line = line || 0
    this.source = source || ''
    this.message = message || 'glsl error'
  }

  public static parse(
    infolog: string | GlslInfoLogRow | Error | null | undefined,
    defaultFilePath?: string,
    filePathLookup?: Readonly<Record<string | number, string>>
  ): GlslInfoLogRow {
    const result = new GlslInfoLogRow()

    if (!infolog) {
      return result
    }
    if (typeof infolog === 'object') {
      if (infolog instanceof GlslInfoLogRow) {
        return Object.assign(result, infolog)
      }
      infolog = `${infolog}`
    }
    if (typeof infolog !== 'string') {
      return result
    }

    const match = _glslInfoLineRegex.exec(infolog)
    if (match) {
      let mType = match[1]
      const filePath = match[2]
      const mLine = match[3]
      const mSource = match[4]
      let mMessage = match[5]
      mMessage =
        typeof mMessage === 'string' ? mMessage : typeof mMessage === 'number' ? `Error ${mMessage}` : 'glsl error'

      if (mMessage.startsWith('Could not process include directive for header name')) {
        mMessage = `Include not found${mMessage.slice('Could not process include directive for header name'.length)}`
      }

      if (mType === 'TBD functionality' || mType === 'Missing functionality') {
        mType = 'ERROR'
        mMessage = mMessage ? `${mType}: ${mMessage}` : mType
      } else if (mType === 'warning' || mType === 'Warnig' || mType === 'warn' || mType === 'Warn') {
        mType = 'WARNING'
      }

      result.type = mType === 'ERROR' || mType === 'WARNING' || mType === 'UNKNOWN ERROR' ? mType : 'ERROR'
      result.filePath = (filePathLookup && filePath && filePathLookup[filePath]) || defaultFilePath || filePath || '0'
      result.line = (typeof mLine === 'number' ? mLine : mLine && Number.parseInt(mLine)) || 0
      result.source = typeof mSource === 'string' ? mSource : ''

      if (!result.source) {
        if (mMessage.startsWith('attribute ')) {
          result.source = 'attribute'
        } else if (mMessage.startsWith('varying ')) {
          result.source = 'varying'
        }
      }

      result.message = mMessage
      return result
    }

    for (const start of ['Warning,', 'Warning.', 'Warning:', 'WARNING:', 'WARNING ']) {
      if (infolog.startsWith(start)) {
        infolog = infolog.slice(start.length).trim()
        result.type = 'WARNING'
      }
    }
    if (result.type === 'ERROR') {
      for (const start of ['Error,', 'Error.', 'Error:', 'ERROR:', 'ERROR ']) {
        if (infolog.startsWith(start)) {
          infolog = infolog.slice(start.length).trim()
        }
      }
    }

    if (defaultFilePath) {
      result.filePath = defaultFilePath
    }
    result.message = infolog

    return result
  }

  public original(): string {
    return _formatOriginalInfoLogRow(this)
  }

  public toString(options?: GlslInfoLogFormatOptions) {
    return _formatGlslInfoLogRow(this, options)
  }
}

;(GlslInfoLogRow.prototype as any)[_util.inspect.custom] = function x(
  _depth?: number,
  options?: NodeJS.InspectOptions
): any {
  return this.toString(options || _util.inspect.defaultOptions)
}

export class GlslInfoLogArray extends Array<GlslInfoLogRow> {
  public clone(): this {
    return this.slice() as this
  }

  public clear() {
    this.length = 0
  }

  public static parse(infolog: string | null | undefined, defaultFilePath?: string): GlslInfoLogArray {
    return new GlslInfoLogArray().parseAdd(infolog, defaultFilePath)
  }

  public parseAdd(
    infolog: GlslInfoLogArray | string | null | undefined,
    defaultFilePath?: string,
    filePathLookup?: Readonly<Record<string | number, string>>
  ): GlslInfoLogArray {
    if (!infolog) {
      return this
    }
    if (infolog instanceof GlslInfoLogArray) {
      this.push(...infolog)
      return this
    }
    const splitted = _splitGlslInfoLog(infolog)
    for (let i = 0, len = splitted.length; i < len; ++i) {
      const row = splitted[i]
      const newItem = GlslInfoLogRow.parse(row, defaultFilePath, filePathLookup)

      // Skip a warning about forced default version
      if (
        newItem.type === 'WARNING' &&
        newItem.message.indexOf('forced to be (') > 0 &&
        newItem.message.indexOf('), while in source code it is') > 0
      ) {
        continue
      }
      this.push(newItem)
    }
    return this
  }

  public getFirstErrorMessage(): string {
    for (let i = 0, len = this.length; i < len; ++i) {
      const item = this[i]
      if (item) {
        if (item.type !== 'WARNING' && item.message) {
          return item.message
        }
      }
    }
    return ''
  }

  public hasErrors(): boolean {
    for (let i = 0, len = this.length; i < len; ++i) {
      const item = this[i]
      if (item && item.type !== 'WARNING') {
        return true
      }
    }
    return false
  }

  public getCounts(): { errors: number; warnings: number } {
    let errors = 0
    let warnings = 0
    for (let i = 0, len = this.length; i < len; ++i) {
      const item = this[i]
      if (item) {
        if (item.type === 'WARNING') {
          ++warnings
        } else {
          ++errors
        }
      }
    }
    return { errors, warnings }
  }

  public original(): string {
    let r = ''
    for (let i = 0, len = this.length; i < len; ++i) {
      const item = this[i]
      if (item) {
        r += `${_formatOriginalInfoLogRow(item)}\n`
      }
    }
    return r
  }

  public inspect(): string {
    return _util.inspect(this, { colors: true })
  }

  public toString(options?: GlslInfoLogFormatOptions): string {
    const chalk =
      options && options.colors ? _chalk : _chalkDisabled || (_chalkDisabled = new _chalk.Instance({ level: 0 }))

    const { errors, warnings } = this.getCounts()
    if (errors === 0 && warnings === 0) {
      return chalk.green('no glsl errors')
    }

    let r = ''
    if (errors > 0) {
      r += `${chalk.redBright(`${errors} error${errors === 1 ? '' : 's'} `)}`
    } else {
      r += `${chalk.gray(`${errors} errors`)} `
    }

    if (warnings > 0) {
      r += `${chalk.yellowBright(`${warnings} warning${warnings === 1 ? '' : 's'}`)}`
    } else {
      r += `${chalk.gray(`${warnings} warnings`)}`
    }
    r += '\n'

    for (let i = 0, len = this.length; i < len; ++i) {
      const item = this[i]
      if (item) {
        r += `  ${_formatGlslInfoLogRow(item, options)}\n`
      }
    }

    return r
  }

  public removeDuplicates(): this {
    const map = new Map()
    for (const row of this) {
      map.set(JSON.stringify(row), row)
    }
    if (map.size !== this.length) {
      this.length = 0
      for (const value of map.values()) {
        this.push(value)
      }
    }
    return this
  }
}

;(GlslInfoLogArray.prototype as any)[_util.inspect.custom] = function x(
  _depth?: number,
  options?: NodeJS.InspectOptions
): any {
  return this.toString(options || _util.inspect.defaultOptions)
}

function _formatOriginalInfoLogRow(row: Readonly<Partial<GlslInfoLogRow>> | null | undefined) {
  if (!row) {
    return ''
  }
  const isError = row.type !== 'WARNING'
  let r = ''
  if (isError) {
    r += row.type || 'ERROR'
  } else {
    r += row.type
  }
  r += ': '
  r += row.filePath || '0'
  r += ':'
  r += row.line || 1
  r += ` "${row.source || ''}": `
  r += row.message || ''
  return r
}

function _formatGlslInfoLogRow(
  row: Readonly<Partial<GlslInfoLogRow>> | null | undefined,
  options?: { colors?: boolean }
) {
  if (!row) {
    return ''
  }

  const chalk =
    options && options.colors ? _chalk : _chalkDisabled || (_chalkDisabled = new _chalk.Instance({ level: 0 }))
  let r = ''

  const isError = row.type !== 'WARNING'

  if (isError) {
    r += chalk.redBright(row.type || 'ERROR')
  } else {
    r += chalk.yellowBright(row.type)
  }
  r += ': '

  r += chalk.cyan(_relativizeFilename(row.filePath) || row.filePath)
  r += ':'

  r += chalk.yellow(row.line || 1)
  r += ` ${chalk.gray("'")}${_escapeErrorStr(row.source, chalk.gray('\\n'))}${chalk.gray("'")}: `

  if (isError) {
    r += chalk.redBright(_escapeErrorStr(row.message, ' '))
  } else {
    r += chalk.yellowBright(_escapeErrorStr(row.message, ' '))
  }

  return r
}

function _splitGlslInfoLog(infolog: string | null | undefined) {
  const result = []
  if (typeof infolog === 'string') {
    let current = ''
    for (const s of infolog.split('\n')) {
      if (_glslInfoLineRegex.test(s)) {
        if (current) {
          result.push(current)
        }
        current = s
      } else {
        const trimmed = s.trim()
        if (!current) {
          current = trimmed
        } else if (trimmed) {
          current += `\n${trimmed}`
        }
      }
    }
    if (current) {
      result.push(current)
    }
  }
  return result
}

function _relativizeFilename(s: any) {
  if (typeof s === 'string' && s.startsWith('/')) {
    return pathRelative(process.cwd(), s)
  }
  return s
}

function _escapeErrorStr(s: any, newLineReplacement: string) {
  if (s === null || s === undefined) {
    return ''
  }
  if (typeof s !== 'string') {
    s = s.toString()
  }
  s = s.replace(/\t/s, '  ').replace(/\r\v\f/s, '')
  s = s === '\n' ? '' : s.replace(/(?:\r\n|\r|\n)/g, newLineReplacement)
  return s
}
