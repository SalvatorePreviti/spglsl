#version 310 es
precision mediump float;
uniform int u;
const int[3] arr = int[3](2, 1, 0);
out vec4 my_FragColor;
void main() {
  switch (u) {
    case arr[1]: my_FragColor = vec4(0.0);
    case 2:
    case 0:
    default: my_FragColor = vec4(1.0);
  }
}

// ShaderValidation FragmentShaderValidationTest, ValueFromConstantArrayAsCaseLabel