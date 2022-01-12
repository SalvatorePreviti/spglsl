import path from 'path'
import util from 'util'
import chalk from 'chalk'
import fastglob from 'fast-glob'

const _conformanceTestLookup = new Map<string, ConformanceTest>()

export class ConformanceTest {
  public readonly filepath: string
  public readonly urlpath: string
  public readonly url: string
  public usesGlsl: boolean = false
  public served: boolean = false
  public errors: string[] = []
  public _durationMs: number = 0
  private _startHrTime: [number, number] = process.hrtime()

  private _finish: ((errors: string[]) => void) | null = null
  private _promise: Promise<string[]> | null = null
  private _finished: boolean = false

  public constructor(filepath: string) {
    this.filepath = filepath
    this.urlpath = path.relative(__dirname, filepath).replace(/\\/, '/')
    this.url = `http://localhost:3000/${this.urlpath}`
  }

  public get finished(): boolean {
    return this._finished
  }

  public get promise(): Promise<string[]> {
    if (this._promise) {
      return this._promise
    }
    if (this._finished) {
      return Promise.resolve(this.errors)
    }
    const p = new Promise<string[]>((resolve) => (this._finish = resolve))
    this._promise = p
    return p
  }

  public getDurationMs() {
    if (!this.served) {
      return 0
    }
    if (this._finished) {
      return this._durationMs
    }
    const hrend = process.hrtime(this._startHrTime)
    return (hrend[0] * 1000000000 + hrend[1]) / 1000000
  }

  public start() {
    if (this.errors.length) {
      this.errors = []
    }
    if (this.errors.length) {
      this.errors = []
    }
    this._finished = false
    this.served = true
    this._startHrTime = process.hrtime()
  }

  public finish() {
    if (!this._finished) {
      this._durationMs = this.getDurationMs()
      this._finished = true
      if (this.served) {
        console.info(
          this.url,
          `${Math.round(this._durationMs)}ms`,
          this.errors.length > 0
            ? chalk.redBright(` FAILED\n${this.errors.map((x) => `  ${x}`).join('\n')}`)
            : chalk.greenBright('Ok'),
          !this.usesGlsl ? chalk.yellowBright('no-glsl') : ''
        )
      }
      if (this._finish) {
        this._promise = null
        this._finish(this.errors)
        this._finish = null
      }
    }
  }

  public [util.inspect.custom]() {
    return {
      url: this.url,
      served: this.served,
      finished: this.finished,
      errors: this.errors
    }
  }

  public toString() {
    return `${this.url} served:${this.served} finished:${this.finished} errors:[${this.errors.join(', ')}]`
  }

  public static get(name: string): ConformanceTest | undefined {
    return _conformanceTestLookup.get(name)
  }
}

export const conformanceTests: ConformanceTest[] = _loadConformanceTests()

function _loadConformanceTests(): ConformanceTest[] {
  const allFiles = new Set(fastglob.sync(path.resolve(__dirname, 'khronos/conformance2/**/*.html')))

  for (const toskip of [
    // needs extensions ifdef
    'khronos/conformance2/extensions/promoted-extensions-in-shaders.html'
  ]) {
    allFiles.delete(path.resolve(__dirname, toskip))
  }

  const filesToMoveAtTheEnd = [
    'khronos/conformance2/glsl3/tricky-loop-conditions.html',
    'khronos/conformance2/state/gl-object-get-calls.html',
    'khronos/conformance2/glsl3/compound-assignment-type-combination.html'
  ]

  for (const fileToMoveAtTheEnd of filesToMoveAtTheEnd) {
    const f = path.resolve(__dirname, fileToMoveAtTheEnd)
    if (allFiles.delete(f)) {
      allFiles.add(f)
    }
  }

  const result = []
  for (const filepath of allFiles) {
    const ct = new ConformanceTest(filepath)
    result.push(ct)
    _conformanceTestLookup.set(ct.filepath, ct)
    _conformanceTestLookup.set(ct.urlpath, ct)
    _conformanceTestLookup.set(ct.url, ct)
  }
  return result
}
