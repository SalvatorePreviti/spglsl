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

export type PrettyFileSizeInput = number | string | Buffer | Uint8Array | null | undefined

export function prettyFileSize(sizeInBytes: PrettyFileSizeInput, fileType?: string) {
  if (sizeInBytes === null || sizeInBytes === undefined) {
    sizeInBytes = 0
  }
  if (typeof sizeInBytes === 'object' || typeof sizeInBytes === 'string') {
    sizeInBytes = utf8ByteLength(sizeInBytes)
  }
  sizeInBytes = roundFromZero(sizeInBytes)
  if (sizeInBytes === 0) {
    return '0 Bytes'
  }
  const bytes = roundFromZero(sizeInBytes)
  const i = bytes && (Math.min(Math.floor(Math.log(Math.abs(bytes) / Math.log(1024))), 6) || 0)
  let s = `${+(bytes / 1024 ** i).toFixed(2)} ${i ? `${' kMGTPE'[i]}B` : 'Bytes'}`
  if (i > 0) {
    s += `${' kMGTPE'[i]}B, ${sizeInBytes} bytes`
  } else {
    s += 'Bytes'
  }
  return fileType ? `${fileType} ${s}` : s
}

function roundFromZero(value: number) {
  return value < 0 ? Math.floor(value) : Math.ceil(value)
}

/** Gets a size in bytes in an human readable form. */
export const humanReadableFileSize = /* @__PURE__ */ (bytes: number): string => {
  bytes = roundFromZero(bytes)
  const i = bytes && (Math.min(Math.floor(Math.log(Math.abs(bytes) / Math.log(1024))), 6) || 0)
  return `${+(bytes / 1024 ** i).toFixed(2)} ${i ? ' kMGTPE'[i] : ''}B`
}
