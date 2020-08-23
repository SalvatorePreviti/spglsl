#version 310 es
precision mediump float;
uniform float u;
float foo(float f) {
  return f * 2.0;
}
float bar(float[2] f) {
  return foo(f[0]);
}
out vec4 my_FragColor;
void main() {
  float arr[2];
  arr[0] = u;
  my_FragColor = vec4(bar(arr));
}

// ShaderValidation FragmentShaderValidationTest, ArrayValueFromFunctionParameterAsParameter