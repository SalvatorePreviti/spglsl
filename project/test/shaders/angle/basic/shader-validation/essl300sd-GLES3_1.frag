#version 310 es
precision mediump float;
in vec4 iv;
out vec4 my_FragColor;

void main() {
  vec4 v4 = vec4(0.0);
  v4 += fwidth(iv);
  v4 += dFdx(iv);
  v4 += dFdy(iv);
  my_FragColor = v4;
}

// ShaderValidation FragmentShaderValidationTest, ESSL300StandardDerivatives