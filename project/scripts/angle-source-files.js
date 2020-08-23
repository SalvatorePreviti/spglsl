#!/usr/bin/env node

// Scripts that generates angle-source-files.txt

const path = require('path')
const fs = require('fs')

const projectDir = path.resolve(__dirname, '../')
const outputFilePath = path.resolve(projectDir, '../angle-make/angle-source-files.txt')

function parseGniFile(gniFilePath, parsed = {}) {
  const compilerGni = fs.readFileSync(path.resolve(projectDir, gniFilePath), 'utf8')

  let isInsideIf = false
  let lineIndex = 0
  let currentName = ''
  for (let line of compilerGni.split('\n')) {
    lineIndex++
    line = line.trim()
    if (!line || line.startsWith('#')) {
      continue
    }
    if (line.startsWith('if')) {
      ++isInsideIf
      continue
    }
    if (isInsideIf) {
      if (line.startsWith('}')) {
        --isInsideIf
        if (isInsideIf < 0) {
          throw new Error(`If stack underflow ${isInsideIf} in ${gniFilePath}:${lineIndex}`)
        }
      }
      continue
    }
    if (line.startsWith('}') || line.startsWith(']')) {
      continue
    }
    if (line.startsWith('[') && line.endsWith(']')) {
      line = line.slice(1, line.length - 2).trim()
    }

    if (line.startsWith('"')) {
      if (!currentName) {
        throw new Error(`String without a given variable name in ${gniFilePath}:${lineIndex}`)
      }
      const filename = line.slice(1).split('"')[0]
      if (!filename) {
        throw new Error(`Invalid string filename in ${gniFilePath}:${lineIndex}`)
      }
      parsed[currentName].push(filename)
      continue
    }

    const indexOfEqual = line.indexOf('=')
    if (indexOfEqual <= 0) {
      throw new Error(`Equal operator not found in ${gniFilePath}:${lineIndex} ${line}`)
    }
    currentName = line.slice(0, indexOfEqual).trim()
    if (currentName.endsWith('+')) {
      currentName = currentName.slice(0, currentName.length - 1).trim()
    }
    if (!currentName) {
      throw new Error(`String without a given variable name in ${gniFilePath}:${lineIndex}`)
    }
    if (!parsed[currentName]) {
      parsed[currentName] = []
    }
  }

  if (isInsideIf !== 0) {
    throw new Error(`If stack overflow ${isInsideIf} in ${gniFilePath}`)
  }
  return parsed
}

const parsedGni = {
  libangle_common_sources: [],

  angle_translator_exported_headers: [],
  angle_translator_sources: [],
  angle_translator_essl_sources: [],
  angle_translator_glsl_sources: [],
  angle_translator_hlsl_sources: [],
  angle_translator_lib_vulkan_sources: [],
  angle_translator_lib_metal_sources: [],
  angle_preprocessor_sources: []
}

parseGniFile('./../angle/src/libGLESv2.gni', parsedGni)
parseGniFile('./../angle/src/compiler.gni', parsedGni)

//console.log(parsed)

const glslSourcesSet = new Set([
  ...parsedGni.libangle_common_sources,
  ...parsedGni.angle_translator_sources,
  ...parsedGni.angle_preprocessor_sources,
  //...parsedGni.angle_translator_essl_sources,

  'src/common/system_utils_posix.cpp'
])

const glslSources = Array.from(glslSourcesSet).map((filename) => path.posix.join('angle', filename))
let glslCppFiles = glslSources.filter(
  (filename) => filename.endsWith('.c') || filename.endsWith('.cpp') || filename.endsWith('.cc')
)

const glslCppFilesSet = new Set(glslCppFiles)

////////////////////////////////////////////////////////////////////////
// Remove files that we know are not really used
////////////////////////////////////////////////////////////////////////
glslCppFilesSet.delete('angle/src/common/android_util.cpp')
glslCppFilesSet.delete('angle/src/common/third_party/base/anglebase/sha1.cc')
glslCppFilesSet.delete('angle/src/common/event_tracer.cpp')
glslCppFilesSet.delete('angle/src/common/system_utils_posix.cpp')
glslCppFilesSet.delete('angle/src/compiler/translator/CodeGen.cpp')
glslCppFilesSet.delete('angle/src/common/aligned_memory.cpp')
glslCppFilesSet.delete('angle/src/common/third_party/smhasher/src/PMurHash.cpp')
glslCppFilesSet.delete('angle/src/common/PackedEGLEnums_autogen.cpp')
glslCppFilesSet.delete('angle/src/common/PackedGLEnums_autogen.cpp')
glslCppFilesSet.delete('angle/src/common/debug.cpp')
////////////////////////////////////////////////////////////////////////

glslCppFiles = Array.from(glslCppFilesSet)

console.log(`Found ${glslCppFiles.length} cpp files`)

const contentToWrite = `${glslCppFiles.map((x) => path.join('../', x)).join('\n')}\n`

console.log('Output file is ./', path.relative(projectDir, outputFilePath))

let isDifferent = true
try {
  if (fs.readFileSync(outputFilePath, 'utf8') === contentToWrite) {
    isDifferent = false
  }
} catch (_) {}

if (isDifferent) {
  fs.writeFileSync(outputFilePath, contentToWrite, 'utf8')
  console.log('file written')
} else {
  console.log('already up to date')
}
