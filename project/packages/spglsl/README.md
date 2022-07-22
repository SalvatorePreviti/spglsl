# spglsl - a webgl glsl minifier based on Google Angle

Execuce preprocessor directives, removes whitespaces, applies constant folding, mangle non exported functions and variable names.
Used in https://github.com/SalvatorePreviti/js13k-2020

## Installation

Package is precompiled at https://www.npmjs.com/package/spglsl

```sh
npm i spglsl --save
```

## Usage

```js
import { spglslAngleCompile } from "spglsl";

async function compileFile() {
  const result = await spglslAngleCompile({
    mainFilePath: "myfile.frag",
    mainSourceCode: fs.readFileSync("myfile.frag", "utf8"),

    minify: true,

    // Mangle everything, except uniforms and globals, "main" and function starting with "main"
    mangle: true,

    // "reserved_word" will not be mangled
    mangle_reserved: ["reserved_word"],

    // Map of global variables to rename
    mangle_global_map: {
      my_uniform_to_rename: "x",
      my_fragment_input_to_rename: "y",
    },
  });

  if (!result.valid) {
    console.log(result.infoLog);
    throw new Error("compilation failed");
  }

  // Prints all the uniform used, key is the original uniform name and value is the renamed uniform, if it was defined in mangle_global_map
  console.log(result.uniforms);

  // Prints all the globals used, key is the original variable name and value is the renamed uniform, if it was defined in mangle_global_map
  // Globals are varying, vertex and fragment input, outputs and buffers ...
  // Globals do not include uniforms.
  console.log(result.globals);

  return result.output;
}
```

There is also an handy rollup and vite plugin:

```js
import { rollupPluginSpglsl } from "spglsl";

const pluginToPassToRollupOrVite = rollupPluginSpglsl({ ...options });
```

## License

MIT license
Salvatore Previti Copyright 2020-2022

Angle Copyright 2018 The ANGLE Project Authors.

## Development

The project is mostly C++, compiled to WASM with Emscripten and runnable in NodeJS.
Look at project/scripts folder on how to install dependencies and build.
