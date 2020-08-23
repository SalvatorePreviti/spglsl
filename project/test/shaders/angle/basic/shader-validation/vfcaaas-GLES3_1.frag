#version 310 es
precision mediump float;
uniform int u;
const int[3] arr = int[3](0, 2, 0);
const int[arr[1]] arr2 = int[2](2, 1);
out vec4 my_FragColor;
void main() {
  my_FragColor = vec4(arr2[1]);
}

// ShaderValidation FragmentShaderValidationTest, ValueFromConstantArrayAsArraySize