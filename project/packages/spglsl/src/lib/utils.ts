import { posix } from 'path'

export function makePathRelative(filePath: string, cwd: string = process.cwd()) {
  if (filePath.indexOf('\\') >= 0) {
    return filePath // avoid doing this on windows
  }
  const relativePath = posix.normalize(posix.relative(filePath, posix.normalize(cwd)))
  return relativePath && relativePath.length < filePath.length ? relativePath : filePath
}

export function prettyGlslFileSize(glsl: string) {
  const bytes = Buffer.byteLength(glsl, 'utf8')
  const i = bytes && (Math.min(Math.floor(Math.log(Math.abs(bytes) / Math.log(1024))), 6) || 0)
  const s = `${+(bytes / 1024 ** i).toFixed(2)} ${i ? ' kMGTPE'[i] : ''}B`
  return bytes >= 1024 ? `${s}, ${bytes} bytes` : `${s}ytes`
}
