#version 300 es
precision mediump float;
uniform int u;

out vec4 my_FragColor;

void main() {
  // Test assigning a constructor result as opposed to initializing with a
  // constructor result.
  int a[3];
  a = int[3](0, 1, u);
  bool fail = false;
  for (int i = 0; i < 2; ++i) {
    if (a[i] != i) {
      fail = true;
    }
  }
  if (a[2] != u) {
    fail = true;
  }
  my_FragColor = vec4(0.0, (fail ? 0.0 : 1.0), 0.0, 1.0);
}
