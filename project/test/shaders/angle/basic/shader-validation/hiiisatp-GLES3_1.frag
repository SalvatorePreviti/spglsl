#version 310 es
precision mediump float;
out vec4 my_FragColor;
void main() {
  if (true)
    if (false)
      my_FragColor = vec4(0.0);
}

// ShaderValidation FragmentShaderValidationTest, HandleIfInnerIfStatementAlwaysTriviallyPruned