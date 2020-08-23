#version 310 es
precision mediump float;
const float a = 1.0;
float b = a * 2.0;
out vec4 my_FragColor;
void main() {
  my_FragColor = vec4(b);
}

// ShaderValidation FragmentShaderValidationTest, AssignConstGlobalToGlobal