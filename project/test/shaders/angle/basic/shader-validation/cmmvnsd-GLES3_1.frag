#version 310 es
precision mediump float;
out vec4 my_FragColor;
void main() {
  mat2x4 foo;
  foo *= mat2x2(4.0);
  my_FragColor = vec4(0.0);
}

// ShaderValidation FragmentShaderValidationTest, CompoundMultiplyMatrixValidNonSquareDimensions