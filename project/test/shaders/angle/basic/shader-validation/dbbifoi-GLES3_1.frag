#version 310 es
precision mediump float;
invariant layout(location = 0) out vec4 my_FragColor;
invariant layout(location = 1) out vec4 my_FragData;
void main() {
  my_FragColor = vec4(0.0);
}

// ShaderValidation FragmentShaderValidationTest, DeclareBothBuiltInFragmentOutputsInvariant