#version 310 es
precision mediump float;
out vec4 my_FragColor;
void main() {
  int i = 0;
  for (int; i < 3; i++) {
    my_FragColor = vec4(i);
  }
}

// ShaderValidation FragmentShaderValidationTest, EmptyDeclarationAsLoopInit