# spglsl - a webgl glsl minifier for NodeJS based on Google Angle

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
