#version 310 es
precision mediump float;
out vec4 my_FragColor;
uniform int i;
void main() {
  float f = vec2[1](vec2(0.0, 0.1))[i].x;
  my_FragColor = vec4(f);
}

// ShaderValidation FragmentShaderValidationTest, ShaderSelectingFieldOfVectorIndexedFromArray