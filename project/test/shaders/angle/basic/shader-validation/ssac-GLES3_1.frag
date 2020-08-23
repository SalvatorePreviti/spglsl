#version 310 es
precision mediump float;
out vec4 my_FragColor;
uniform float u;
struct S {
  float member;
};
void main() {
  S[1] sarr = S[1](S(u));
  my_FragColor = vec4(sarr[0].member);
}

// ShaderValidation FragmentShaderValidationTest, SingleStructArrayConstructor