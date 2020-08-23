#version 310 es
precision mediump float;
uniform int u;
const int[3] arr = int[3](4, 1, 0);
out vec4 my_FragData[2];
void main() {
  my_FragData[arr[1]] = vec4(0.0);
  my_FragData[arr[2]] = vec4(0.0);
}

// ShaderValidation FragmentShaderValidationTest, ValueFromConstantArrayAsFragmentOutputIndex