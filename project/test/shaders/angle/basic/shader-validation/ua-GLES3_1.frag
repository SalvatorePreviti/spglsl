#version 310 es
precision mediump float;
uniform vec4[2] u;
out vec4 my_FragColor;
void main() {
  my_FragColor = u[0];
}

// ShaderValidation FragmentShaderValidationTest, UniformArray