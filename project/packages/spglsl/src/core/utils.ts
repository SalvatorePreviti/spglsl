import { posix } from 'path'

export function makePathRelative(filePath: string, cwd?: string) {
  if (!filePath) {
    return './'
  }
  if (filePath.indexOf('\\') >= 0) {
    return filePath // avoid doing this on windows
  }
  try {
    const relativePath = posix.normalize(posix.relative(cwd || process.cwd(), filePath))
    return relativePath && relativePath.length < filePath.length ? relativePath : filePath
  } catch (_) {
    return filePath
  }
}

export function utf8ByteLength(b: Uint8Array | Buffer | string | number | null | undefined) {
  if (b === null || b === undefined) {
    return 0
  }
  if (typeof b === 'number') {
    return b || 0
  }
  return typeof b === 'string' ? Buffer.byteLength(b, 'utf8') : b.length
}

export type PrettySizeInput = number | string | Buffer | Uint8Array | null | undefined

export function bufferToUtf8(b: Uint8Array | Buffer | string) {
  if (typeof b === 'string') {
    return b
  }
  if (Buffer.isBuffer(b)) {
    return b.toString('utf8')
  }
  return Buffer.from(b).toString('utf8')
}

export interface PrettySizeOptions {
  appendBytes?: boolean
  fileType?: string
}

/** Gets a size in bytes in an human readable form. */
export function prettySize(bytes: PrettySizeInput, options?: PrettySizeOptions): string {
  if (bytes === null || bytes === undefined) {
    bytes = 0
  }
  const appendBytes = !options || options.appendBytes === undefined || options.appendBytes
  if (typeof bytes === 'object' || typeof bytes === 'string') {
    bytes = utf8ByteLength(bytes)
  }
  bytes = bytes < 0 ? Math.floor(bytes) : Math.ceil(bytes)
  let s: string
  if (!isFinite(bytes) || bytes < 1024) {
    s = `${bytes} ${appendBytes ? 'Bytes' : 'B'}`
  } else {
    const i = Math.min(Math.floor(Math.log(Math.abs(bytes)) / Math.log(1024)), 6)
    s = `${+(bytes / 1024 ** i).toFixed(2)} ${i ? ' kMGTPE'[i] : ''}`
    if (appendBytes) {
      s += `, ${bytes} Bytes`
    }
  }
  if (options && options.fileType) {
    s = `${options.fileType} ${s}`
  }
  return s
}
