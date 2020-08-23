#version 310 es
precision mediump float;
out vec4 my_FragColor;
void main() {
  const float f = vec2[1](vec2(0.0, 1.0))[0].x;
  my_FragColor = vec4(f);
}

// ShaderValidation FragmentShaderValidationTest, FieldSelectionFromVectorArrayConstructorIsConst