# spglsl - a webgl glsl minifier for NodeJS based on Google Angle

The project is mostly C++, compiled to WASM with Emscripten and runnable in NodeJS.

Package is precompiled at https://www.npmjs.com/package/spglsl

## Installation

```sh
npm i spglsl --save
```

## Usage

```js
import { spglslAngleCompile } from 'spglsl'

async function compileFile() {
  const result = await spglslAngleCompile({
    mainFilePath: 'myfile.frag',
    mainSourceCode: fs.readFileSync('myfile.frag', 'utf8'),
    minify: true
  })

  if (!result.valid) {
    console.log(result.infoLog)
    throw new Error('compilation failed')
  }

  return result.output
}
```

## License

MIT license
Salvatore Previti Copyright 2020-2021

Angle Copyright 2018 The ANGLE Project Authors.

## Development

Look at project/scripts folder on how to install dependencies and build.
