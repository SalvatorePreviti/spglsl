#version 310 es
precision mediump float;
out vec4 my_FragColor;
struct S {
  float member;
};
void main() {
  const float f = S[1](S(0.0))[0].member;
  my_FragColor = vec4(f);
}

// ShaderValidation FragmentShaderValidationTest, FieldSelectionFromStructArrayConstructorIsConst