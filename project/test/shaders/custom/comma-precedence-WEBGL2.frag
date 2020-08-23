#version 310 es
precision mediump float;

out vec4 my_FragColor;

int[2] func(int param) {
  return int[2](param, param);
}

void main() {
  int a[2];
  for (int i = 0; i < 2; ++i) {
    a[i] = 1;
  }
  int j = 0;
  // Sequence operator evaluates operands from left to right (ESSL 3.00 section 5.9).
  // The function call that returns the array needs to be evaluated after ++j
  // for the expression to return the correct value (true).
  bool result = ((++j), (a == func(j)));
  my_FragColor = vec4(0.0, (result ? 1.0 : 0.0), 0.0, 1.0);
}