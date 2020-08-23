#version 310 es
precision mediump float;
out vec4 my_FragColor;
float groupMemoryBarrier() {
  return 1.0;
}

void main() {
  my_FragColor = vec4(groupMemoryBarrier());
}

// ShaderValidation FragmentShaderValidationTest, RedeclaringBuiltInFromAnotherShaderStage