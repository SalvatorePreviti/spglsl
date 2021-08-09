import { floatToGlsl } from '../core/float-math'

let _wasmSpglsl: any = null
let _wasmSpglslPromise: Promise<any> | null = null

export async function spglslPreload() {
  if (!_wasmSpglsl && !_wasmSpglslPromise) {
    await _wasmSpglslGet()
  }
}

export function spglslUnload() {
  if (_wasmSpglsl) {
    _wasmSpglsl = null
    _wasmSpglslPromise = null
    return true
  }
  return false
}

export async function _wasmSpglslGet(): Promise<{ spglsl: any }> {
  return {
    spglsl: _wasmSpglsl || (await (_wasmSpglslPromise || (_wasmSpglslPromise = _spglslInit())))
  }
}

async function _spglslInit() {
  let result
  try {
    const dynamicImport = '../../wasm/spglsl.js'
    const imported = await import(dynamicImport)
    result = await (imported.default || imported)()
    if (
      !result.spglsl_init({
        floatToGlsl
      })
    ) {
      throw new Error('spglsl initialization failed')
    }
    _wasmSpglsl = result
    return result
  } catch (cause) {
    _wasmSpglslPromise = null
    const error = cause instanceof Error ? cause : new Error(cause)
    throw error
  }
}
