#version 300 es
precision mediump float;

out vec4 my_FragColor;

void main() {
  // This simple test uses the ESSL1 style array initialization in order
  // to be able to test array equality independently of array constructors.
  int a[3];
  int b[3];
  int c[3];
  for (int i = 0; i < 3; ++i) {
    a[i] = i;
    b[i] = i;
    c[i] = i + 1;
  }
  bool success = (a == b) && (a != c);
  my_FragColor = vec4(0.0, (success ? 1.0 : 0.0), 0.0, 1.0);
}
